# MyCppProject

SDL2を使ったC++プロジェクト

## 必要な環境

- Windows 10/11
- Visual Studio 2022
- CMake 3.20以上
- vcpkg

## セットアップ

### 1. vcpkgのインストール

vcpkgがまだインストールされていない場合:

```powershell
# vcpkgをインストール
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat

# 環境変数を設定
setx VCPKG_ROOT "C:\vcpkg"
```

PowerShellとVisual Studioを再起動してください。

### 2. プロジェクトのクローン

```powershell
git clone <このリポジトリのURL>
cd MyCppProject
```

### 3. Visual Studioで開く

1. Visual Studio 2022を起動
2. 「ファイル」→「開く」→「フォルダ」
3. `MyCppProject`フォルダを選択
4. CMakeが自動的に依存関係をダウンロード・ビルド(初回は時間がかかります)

### 4. ビルドと実行

- ビルド: `Ctrl+Shift+B`
- 実行: `Ctrl+F5`

## 使用ライブラリ

- SDL2: ウィンドウ管理・グラフィックス
- SDL2_image: 画像読み込み
- fmt: フォーマット出力

## ライブラリの追加方法

```powershell
# プロジェクトルートで
vcpkg add port <ライブラリ名>
```

その後、`CMakeLists.txt`に追加:
```cmake
find_package(<ライブラリ名> CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE <ライブラリ名>::<ライブラリ名>)
```