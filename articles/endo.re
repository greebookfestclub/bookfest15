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

図 従来フロー
TODO シーケンス図
図 現状の新フロー
TODO シーケンス図

=== 受信したレシートが正当か検証し、情報ß取得する


レシート
方法はこちらの動画でも紹介されています。
https://developer.apple.com/videos/play/wwdc2023/10143/
apple公式ライブラリが用意されている言語ではそれにしたがってください。
証明書はこちら
https://www.apple.com/certificateauthority/
今使っているのはphpのためそちらで考えます。参考にしたライブラリはこちらです。
レシートのデータは、ASN.1 というフォーマットです。ここから証明書チェーンを検証して最終的にAppleのサイトにある証明書と検証することで、内容が正しいことを確認します。
データの中身を取り出す時も、ASN.1の中身を探していきます。
取り出すための定義はこちらにあります。
https://developer.apple.com/library/archive/releasenotes/General/ValidateAppStoreReceipt/Chapters/ReceiptFields.html


レシートから情報を取り出す
こんな感じです。

===  AppleのApp Store Server APIにより情報を取得する
API
https://developer.apple.com/documentation/appstoreserverapi
ここまででtransactionIdが入手できたので、これを使って課金情報向けのAPIを呼び出します。


取り出せない情報について
レシートに入っているのは発行時点の情報です。ですので定期課金などにおいては欲しい情報がない場合があります。
例えば
・TODO
といった情報はレシートに入っていません。そういった情報はAPI経由で取得することにしましょう。

== まとめ
Storekit 2に移行するか、もしくは頑張ってverfiyReceiptのdeprecatedに対応しましょう。



