# 🌿 Leaf Chat 🌿

A minimal, lightweight WhatsApp Web client for Ubuntu built with pure C, GTK3, and WebKit2GTK.

## Short Description

Leaf Chat is an ultra-minimal WhatsApp Web client designed for Ubuntu. It provides a native desktop experience for WhatsApp Web with the smallest possible binary size and clean, distraction-free interface.

## Long Description

Leaf Chat is a lightweight desktop application that wraps WhatsApp Web in a native GTK3 window. Built entirely in C without any object-oriented wrappers or external dependencies beyond GTK3 and WebKit2GTK, it offers:

- **Minimal footprint**: Optimized for the smallest possible binary size with aggressive compiler and linker optimizations
- **Native integration**: Seamless desktop experience with proper window management and system integration
- **Data persistence**: Automatic storage of cookies, local storage, IndexedDB, and cache in `~/.local/share/leaf-chat/`
- **File downloads**: Full support for downloading files from WhatsApp with native file chooser dialogs
- **Progress tracking**: Real-time download progress displayed in a status bar
- **Modern user-agent**: Configured to prevent WhatsApp Web misidentification issues
- **Clean interface**: Single window, no menus, no headerbars - just WhatsApp Web

The application is designed to be as lightweight as possible while maintaining full functionality. The entire codebase is under 130 lines of well-commented C code, making it easy to understand and modify.

![First look: Login view](https://i.imgur.com/Wou6wyR.png)

## Dependencies

### Build Dependencies

- **CMake** (3.10 or higher)
- **pkg-config**
- **GCC** or compatible C compiler
- **GTK3 development libraries** (`libgtk-3-dev`)
- **WebKit2GTK development libraries** (`libwebkit2gtk-4.1-dev`)

### Runtime Dependencies

- **GTK3** runtime libraries
- **WebKit2GTK** runtime libraries

## Installation

### Install Build Dependencies on Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y cmake pkg-config libgtk-3-dev libwebkit2gtk-4.1-dev
```

## Compile Instructions

1. Clone or download the project:
```bash
git clone <repository-url>
cd Leaf-Chat
```

2. Build the project:
```bash
cmake -B build
cmake --build build
```

The optimized binary will be created at `build/whatsapp`.

### Build Options

The project is configured for minimal binary size by default with the following optimizations:
- `-Os` - Optimize for size
- `-s` - Strip symbols
- `-ffunction-sections -fdata-sections` - Enable dead code elimination
- `-Wl,--gc-sections` - Remove unused sections
- `-g0` - No debug information

## How to Run

After building, run the application:

```bash
./build/whatsapp
```

The application will:
- Open a 1920x1080 window titled "🌿 Leaf Chat 🌿"
- Automatically load WhatsApp Web
- Store all data in `~/.local/share/leaf-chat/`

### Optional: Icon Support

If you have an `app.png` file in the project directory, it will be automatically used as the window icon. The application will continue to work normally if the icon file is missing.

## How to Package Minimal .deb File

To create a minimal Debian package, follow these steps:

### 1. Install Packaging Tools

```bash
sudo apt-get install -y devscripts debhelper dh-make
```

### 2. Create Package Structure

```bash
mkdir -p leaf-chat-1.0.0
cd leaf-chat-1.0.0
mkdir -p debian/usr/bin
mkdir -p debian/usr/share/applications
mkdir -p debian/usr/share/pixmaps
```

### 3. Copy Files

```bash
# Copy the binary
cp ../build/whatsapp debian/usr/bin/leaf-chat

# Create desktop entry (optional)
cat > debian/usr/share/applications/leaf-chat.desktop << EOF
[Desktop Entry]
Name=Leaf Chat
Comment=Minimal WhatsApp Web client
Exec=leaf-chat
Icon=leaf-chat
Type=Application
Categories=Network;InstantMessaging;
EOF

# Copy icon if available (optional)
# cp ../app.png debian/usr/share/pixmaps/leaf-chat.png
```

### 4. Create DEBIAN Control File

```bash
mkdir -p debian/DEBIAN
cat > debian/DEBIAN/control << EOF
Package: leaf-chat
Version: 1.0.0
Section: net
Priority: optional
Architecture: amd64
Depends: libgtk-3-0, libwebkit2gtk-4.1-0
Maintainer: Hasan <hasanimroz.personal@gmail.com>
Description: Minimal WhatsApp Web client
 A lightweight desktop application that wraps WhatsApp Web
 in a native GTK3 window. Built with pure C for minimal
 binary size and maximum performance.
EOF
```

### 5. Set Permissions

```bash
chmod 755 debian/usr/bin/leaf-chat
chmod 644 debian/usr/share/applications/leaf-chat.desktop
```

### 6. Build the Package

```bash
dpkg-deb --root-owner-group --build Leaf-Chat
```

### 7. Install the Package

```bash
sudo dpkg -i Leaf-Chat.deb
```

If there are missing dependencies, install them with:

```bash
sudo apt-get install -f
```

## Gallery
- Conversation list and main UI:
![Conversation list and main UI](https://i.imgur.com/0ujIHPM.png)

- Chat window with messages:
![Chat window with messages](https://i.imgur.com/RQvbCXQ.png)

- Chat themes:
![Chat themes](https://i.imgur.com/GfPhfNW.png)

- Keyboard shortcuts:
![Keyboard shortcuts](https://i.imgur.com/BuuVAQT.png)


## License

[MIT License]
