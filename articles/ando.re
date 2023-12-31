= ゲームアプリ移管時に押さえておきたいポイント
== はじめに
私はグリーエンターテインメントの前身であるファンプレックス時代から4つのタイトルのアプリ移管(引き継ぐ側)を経験し、また、横断チームとしてその他のアプリ移管のフォローを行ってきました。
@<br>{}
それらの経験で培った”ゲームアプリ移管時に押さえておきたいポイント”を紹介したいと思います。
@<br>{}
@<br>{}
なお、本内容は以下のURLからでもご覧いただけます。
@<br>{}
https://gree-entertainment.com/topics/4838/@<br>{}

== 前提
今回紹介する内容は以下を前提として記載します。

 * ゲームエンジンはUnityを使用
 * パブリッシャー様が存在する開発移管である
 * 課金システムなどはパブリッシャー様のシステムやSDKを使用している
 * アプリ移管の際、移管元の開発会社様から情報を収集できる期間が1〜2ヶ月程ある（以下、この期間のことを”移管時”と表現します）
 * 移管元の開発会社様の作成物の使用権利などは契約時点で完結している

//indepimage[ando_image_1][パブリッシャー様と移管元の会社様との関係図]

== 押さえておきたいポイント
早速ですが、アプリ移管時に押さえておきたいポイントを記載します。@<br>{}

 * 機材調達とゲームエンジンやToolのバージョン
 * 使用しているPluginやライブラリの出処
 * ライセンス再購入が必要なAssetやTool類
 * 3rdパーティのソースコードの編集有無

以下に詳細を記載します。

=== 機材調達とゲームエンジンやToolのバージョン
こちらは移管前の準備段階の話になります。@<br>{}
開発に使用するPCはアプリによって異なり、MacPC or WindowsPC、PCスペックも様々です。MacPCだとそこまで選択肢はありませんが、WindowsのHighスペックなグラフィックボードを積んだPCでなければならない場合もあります。移管前には開発者全員分のPCを揃える必要がありますので、早めの調達を心掛けます。@<br>{}
また、自分達の作業用PCの他にJenkinsなどで使用するPCも準備する必要があるかと思います。iOSアプリを作成する場合はMacPCが必須となりますので、こちらの準備も忘れないようにします。@<br>{}
@<br>{}
PCと合わせて使用するゲームエンジンやToolのバージョンなども事前に移管元の開発会社様から聞いておくとスムーズです。情報入手後はそれらのバージョンを入手＆インストールして、移管作業に臨みます。@<br>{}
もし、最新のバージョンよりかなり古いものを使用している場合は注意が必要です。古いバージョンを入手できない場合もあります。また、サポート切れのツールを使用している場合も対策が必要です。入手可能なバージョンを使用できるか？などを関係各所と検討する必要があります。@<br>{}

=== 使用しているPluginやライブラリの出処
アプリ移管の際には使用しているPluginやライブラリを必ず洗い出します。@<br>{}
使用されているものの中で、メジャーなものは調査すれば出処などの各種情報を入手可能ですが、移管元の会社様のエンジニアが独自に作成したものの場合は移管時に情報を取得する必要があります。@<br>{}
もし、移管後にその事が発覚し、1から調査するとなるとかなりの労力が必要です。調査する際、ソースコードの場合は読めば良いのですが、AndroidのJarファイル(ライブラリ)などはリバースエンジニアリング等でソースコードを掘り起こす必要が出てきます。@<br>{}
そうならない為にも移管元の会社様で作成したものは必ず把握し、ライブラリやツールなどでバイナリファイルしかないものについては、可能な限りそれに対するソースコードとビルド手順も引き継ぐ様にします。@<br>{}
@<br>{}
また、パブリッシャー様のSDKやPluginの情報なども早めに把握しておくと良いです。パブリッシャー様によっては移管後でも詳細な情報を得ることが可能ですが、そうでないことも多いです。更に移管時にパブリッシャー様のサポート体制やSDK、Pluginなどの受領方法などを把握し、移管元のエンジニアにSDKなどの組み込み手順やポイントなどを聞いておくことも大切です。@<br>{}

==== ライセンス購入が必要なAssetやTool類
UnityのAssetStoreで購入可能なAssetには2種類（3通り）のライセンス種別が存在します。

 * シートライセンス
 * サイトライセンス
 ** Single Entity（単一の法人または個人向け）
 ** Multi Entity（複数の法人向け）

どちらのライセンスだとしても、アプリ移管の際には再購入が必要となります。@<br>{}
ライセンスの情報に関しては自分達で調査可能かもしれませんが、念には念を入れて移管元の会社様からも情報を取得しておく事が大事です。また、現場レベルでは把握していない可能性もありますので、その場合は責任を取れる役職の方に確認しておくことも大切です。@<br>{}
@<br>{}
UnityAsset以外でも便利ツールなどを使用する際、購入を必要とするものが存在しますので、忘れないように注意しています。@<br>{}
この辺りは企業のコンプライアンスに関わることなので、漏れなく情報を集めて、確実に対処することが重要です。

==== 3rdパーティのソースコードの編集有無
Unityでアプリを作成する際、3rdパーティから提供されているPluginやソースコードを利用することは一般的なことです。@<br>{}
アプリを作成する際、提供されている内容をそのまま使用する事が多いのですが、アプリの細かな仕様やパフォーマンスに対応する為、提供されたソースコードを直接編集しているアプリも少なくありません。その場合は可能な限り、どこをどの様に編集したか？などの情報を取得しておく必要があります。@<br>{}
@<br>{}
移管直後は問題ないかもしれませんが、将来3rdパーティのPluginやソースコードを更新する事になった場合、移管元で編集された内容を反映＆検証する必要が出てきます。もし、編集されていることを知らずに3rdパーティのソースコードだけ更新してしまうとアプリの動作が変わったり、不具合の原因になります。@<br>{}
@<br>{}
編集された内容を自分達だけで調査するのは非常にコストがかかります。将来のコスト削減の為にも可能な限り移管時に移管元のエンジニアへ確認しておくと良いです。@<br>{}
また、Unity内のファイルのAssets/Plugins/iOS内のmmファイルなどを変更しているケースも見受けられますので注意が必要です。@<br>{}

== 最後に
細かいポイントを上げればキリがないので、今回は影響の大きいものを紹介しました。@<br>{}
私もアプリの移管元(引き渡す側)になった事があります。苦労して制作してきたアプリを引き渡す際は寂しい思いでした。@<br>{}
引き渡す側のそんな思いを払拭する為にも、引き継ぐ側は事前の準備をしっかり行い、IPやゲームアプリに熱意を持ち、パブリッシャー様や移管元の会社様が安心する様なゲームアプリ移管を今後も実現していきたいと思います。@<br>{}


