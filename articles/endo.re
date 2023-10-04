= iOS課金検証 verifyReceipt APIのdeprecatedへの対応
== 経緯
従来からApple課金の内容把握やチェックにはverifyReceipt API@<fn>{endofn01}を使う必要がありました。サーバーサイドでレシート検証を行うシステムの場合、こちらを利用していることでしょう。
//footnote[endofn01][https://developer.apple.com/documentation/appstorereceipts/verifyreceipt]
しかし先日のWWDC23にてverifyReceitp APIはdeprecatedになりました。ではどのように今後のレシート検証を行えば良いのでしょうか？
== レシート検証
アプリの動作対象がiOS15以上のみの場合はStorekit 2への移行が基本的な方向になるでしょう。Storekit 2では旧来のレシートを扱わないため、verifyReceipt APIも使いません。	サーバーとの間ではJWSをやり取りし、その検証とApp Store Server APIによる情報取得による方法がメインとなりそうです。
ではiOS15未満で動作する可能性がある場合や、Storekit 2への移行がまだ先の場合はどのように対応していきましょう。すなわち、クライアント側は従来通りで、サーバー側でverifyReceipt APIを使わない方法について考えていきます。ドキュメント上は、クライアントサイドのみで検証する方法とApp Store Server APIを使う方法が示されています。(デバイスのみで完結したい場合はクライアントサイドでの検証を使う場合もあると思われますが今回こちらは取り上げません@<fn>{endofn02} )。今回はサーバーサイドでの検証について考えていきます。
//footnote[endofn02][https://developer.apple.com/documentation/appstorereceipts/validating_receipts_on_the_device]
=== 流れ
サーバーサイドで検証を行う方法は、今回は次のようになる想定です。
* クライアントからサーバーへレシートを送信する
* サーバーで受信したレシートが正当か検証する
* サーバーで受信したレシートから課金情報を取得する
* (必要な場合) AppleのApp Store Server APIにより情報を取得する

//indepimage[endo_sequence01][従来フロー]{
App->Server: レシート
Server->Apple: verifyReceipt API
Apple->Server: 検証結果,情報
Server->Server: 反映
Server->App:
//}

//indepimage[endo_sequence02][新フロー]{
App->Server: レシート
Server->Server: レシート検証,情報取得
Server->Apple: App Store Server API
Apple->Server: 情報
Server->Server: 反映
Server->App:
//}

=== 受信したレシートが正当か検証し、情報取得する
方法はこちらの動画でも紹介されています@<fn>{endofn03}。今回はphpを想定しているので自前で実装していきますが、動画にあるようにApple公式ライブラリが用意されている言語(Swift, Java, Node.js, Python)ではそれにしたがってください。
//footnote[endofn03][https://developer.apple.com/videos/play/wwdc2023/10143/]
証明書はあらかじめダウンロードしておきます@<fn>{endofn04}。
//footnote[endofn04][https://www.apple.com/certificateauthority/]
今回参考にしたライブラリはこちらです@<fn>{endofn05}。
//footnote[endofn05][https://github.com/CthulhuDen/ios-receipt-parser]
==== 検証
検証についてはドキュメントではこのあたりに記載があります@<fn>{endofn06}。
//footnote[endofn06][https://developer.apple.com/documentation/appstorereceipts/validating_receipts_on_the_device]
レシート全体をPKCS #7コンテナとして読み取り、証明書チェーンを抽出し、最終的にAppleの証明書と検証を行います。このようなコードが想定されます。
//list[endo-1][php]{
        $pkcs7_text = (
            "-----BEGIN CERTIFICATE-----\n" .
            chunk_split($receipt, 64, "\n") .
            "-----END CERTIFICATE-----\n"
        );
        $result = openssl_pkcs7_read($pkcs7_text, $certificate_list);
        if ($result === false) {
            //receipt cannot change x509.
            return false;
        }
//}

//list[endo-2][php]{
        $pem_list[] = $apple_pem;
        for($counter = 0; $counter < (count($pem_list) - 1); $counter++) {
            $cert = $pem_list[$counter];
            $x509 = new X509();
            $x509->loadCA($pem_list[$counter+1]);
            $x509->loadX509($cert);
            if (!$x509->validateSignature()) {
                //検証に失敗
            }
        }
        //検証に成功
//}

==== 情報取得

レシートのデータは、ASN.1 というフォーマットです。まずデータの中身を取り出す時の定義はこちらです@<fn>{endofn07}。
//footnote[endofn07][https://github.com/CthulhuDen/ios-receipt-parser/blob/master/src/ASN1/Pkcs7UnverifiedParser.php#L18-L56]
デコードにはphpseclib3のASN.1 Parserが良いでしょう@<fn>{endofn08}。
//footnote[endofn08][https://api.phpseclib.com/2.0/phpseclib/File/ASN1.html#method_decodeBER]
payload取得はこのようなイメージになります。
//list[endo-3][php]{
    public static function readPayload(string $ber, array $mapping):?string
    {
        $data = self::decode(base64_decode($ber), $mapping);
        return $data['data']['data']['data'] ?? null;
    }
    public static function decode(string $binary, $type)
    {
        return ASN1::asn1map(
            ASN1::decodeBER($binary)[0],
            is_array($type) ? $type : ['type' => $type]
        );
    }
//}
ここで取得したpayloadをデコードする時にはこのような定義を使います@<fn>{endofn09}。
//footnote[endofn09][https://github.com/CthulhuDen/ios-receipt-parser/blob/master/src/ASN1/SimpleDecoder.php#L38-L57]
payloadを取り出した後、各項目を取り出すためのデータ定義はAppleのサイトにあります@<fn>{endofn10}。
//footnote[endofn10][https://developer.apple.com/library/archive/releasenotes/General/ValidateAppStoreReceipt/Chapters/ReceiptFields.html]
たとえばBundle Identifierが欲しいならその項目を見ると「ASN.1 Field Type 2」「ASN.1 Field Value UTF8STRING」とあるので、この定義に従い情報を取り出します。おおよそこのような感じになります。
//list[endo-4][php]{
    const RECEIPT_BUNDLE_ID = 2;

    public function getBundleId(): string
    {
        return self::decode(
            self::(self::RECEIPT_BUNDLE_ID),
            ASN1::TYPE_UTF8_STRING
        );
    }

    public function get($payload, int $type): ?string
    {
        foreach ($payload as $attribute) {
            if ((int) (string) $attribute['type'] === $type) {
                return $attribute['value'];
            }
        }
        return null;
    }

    public static function decode(string $binary, $type)
    {
        return ASN1::asn1map(
            ASN1::decodeBER($binary)[0],
            is_array($type) ? $type : ['type' => $type]
        );
    }
//}

なお、レシートの中には過去の課金が複数件入っているので、Transaction IdentifierなどはIn-App Purchase Receiptの中に入っています。なのでTransaction Identifierが欲しい場合、まずIn-App Purchase Receiptを取得し、次にTransaction Identifierを取得する、という手順になります。流れを整理すると、このようになります。

===  AppleのApp Store Server APIにより情報を取得する
==== API
ここまでで基本的には必要な情報が入手できています。しかし欲しい情報がレシートに存在しない場合、もしくはその課金に関する最新の情報を取得したい場合もあるため、APIの呼び出しによる情報取得ついても見ていきましょう。
レシートからtransactionIdもしくはoriginalTransactionIdが入手できたので、これを使って課金情報向けのApp Store Server API@<fn>{endofn11}を呼び出します。
//footnote[endofn11][https://developer.apple.com/documentation/appstoreserverapi]
==== APIで取り出せない情報について
レシートに入っているのは発行時点の情報です。ですので定期課金などにおいては欲しい情報がない場合があります。例えば
* Subscription Expiration Intent
* Subscription Retry Flag
* Subscription Trial Period
* Cancellation Reason
* Subscription Auto Renew Status
といった情報はレシートに入っていません。これらは課金発生時点で分からないものなどですので、そういった情報はAPI経由で取得することにしましょう。

== まとめ
Storekit 2に移行するか、もしくは頑張ってverfiyReceiptのdeprecatedに対応しましょう。



