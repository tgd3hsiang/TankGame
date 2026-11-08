# Tank Battle with Enemy AI

C++17 と raylib で制作した、ブラウザ移植を想定した小規模な2D戦車ゲームです。
敵の行動管理に**有限状態機械（FSM）**、障害物を避けた移動に**A*経路探索**を使用しています。

> **Status / 現状**: 最初のプロトタイプ。デスクトップ動作を優先した構成です。Web版・GitHub Pages・CI/CDは今後検証予定です（動作確認前に公開済みと記載しないこと）。

## 操作方法

- `WASD` または方向キー：移動
- マウス：砲塔の向き
- マウス左クリック または `Space`：射撃
- `F1`：AIデバッグ表示の切り替え
- `R`：ゲーム終了後の再開

## 技術的なポイント

- `Tank`：プレイヤーと敵に共通する移動・砲塔操作・発射間隔
- `TileMap`：タイルマップ、壁との衝突判定、視線判定
- `EnemyAI`：Patrol / Chase / Attack の状態遷移
- `Pathfinding`：グリッド上のA*経路探索
- `Game`：ゲームループ、入力、勝敗判定、描画

## ビルド（デスクトップ）

C++17 対応コンパイラ、Git、CMake、raylib のビルドに必要なOS依存パッケージが必要です。
CMake は raylib `5.5` をGitHubから取得します（初回はインターネット接続が必要）。

```bash
cmake -S . -B build
cmake --build build --config Release
```

- Windows / Visual Studio: `build/Release/TankBattleAI.exe`（構成によって異なる）
- Linux/macOS / single-config generator: `build/TankBattleAI`

## WebAssembly（後から検証する手順）

Emscripten SDK を導入し、`emcmake` が使用可能な環境で実行します。

```bash
emcmake cmake -S . -B build-web
cmake --build build-web
```

生成される `TankBattleAI.html` はローカル HTTP サーバー経由で確認してください。
Webブラウザ版は、raylib / Emscripten 環境やビルド設定により追加調整が必要な場合があります。

## 計画

1. デスクトップ版の実機動作を検証
2. AI 状態遷移・A*の挙動を改善し、障害物での詰まりを修正
3. WebAssembly 版をビルド・検証
4. GitHub Actions でビルドと GitHub Pages デプロイを自動化
5. スクリーンショットとプレイ動画を追加

## AIツールの利用について

設計やボイラープレート作成にAI支援を使用しています。提出前に各クラスの役割やアルゴリズムを理解し、自分で改善・動作確認を行います。
