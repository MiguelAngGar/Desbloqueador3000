# Desbloqueador3000

Aplicación de escritorio para eliminar la protección de hojas y libros de archivos Excel (`.xlsx`).  
Funciona arrastrando el archivo a la app — el resultado aparece listo para arrastrar donde quieras.

> ⚠️ **Solo funciona con archivos sin contraseña de apertura.** No rompe cifrado AES.  
> Úsalo únicamente sobre archivos de tu propiedad.

---

## Descarga (macOS)

👉 [Descargar última versión para macOS (.dmg)](../../releases/latest)

Compatible con macOS 12 Monterey o superior, Apple Silicon y Intel.

---

## Cómo funciona

1. Arrastra tu `.xlsx` al panel superior (o haz clic para seleccionarlo)
2. La app extrae el archivo, elimina los nodos `<sheetProtection>` y `<workbookProtection>` del XML interno, y lo reempaqueta
3. El archivo procesado aparece en el panel inferior listo para arrastrarlo donde quieras

---

## Compilar desde código fuente

### Requisitos comunes
- CMake 3.16+
- Qt 6.x (módulos: Widgets, Concurrent)

---

### macOS

```bash
# Instalar dependencias
brew install cmake qt6

# Clonar y compilar
git clone https://github.com/your-username/Desbloqueador3000.git
cd Desbloqueador3000

cmake -B build \
  -DCMAKE_PREFIX_PATH=$(brew --prefix qt6) \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build

# Ejecutar
./build/Desbloqueador3000.app/Contents/MacOS/Desbloqueador3000
```

---

### Windows

**Requisitos:**
- [Qt6 (Community)](https://www.qt.io/download-qt-installer) — instala el componente `MSVC 2022 64-bit`
- [Visual Studio Build Tools 2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022)
- [CMake](https://cmake.org/download/)

```powershell
git clone https://github.com/your-username/Desbloqueador3000.git
cd Desbloqueador3000

cmake -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.x.x\msvc2022_64"
cmake --build build --config Release
```

El ejecutable queda en `build\Release\Desbloqueador3000.exe`.

---

## Estructura del proyecto

```
Desbloqueador3000/
├── CMakeLists.txt
├── include/
│   ├── MainWindow.h
│   ├── DropZone.h
│   ├── ResultPanel.h
│   ├── ExcelProcessor.h
│   └── SpinnerWidget.h
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── DropZone.cpp
│   ├── ResultPanel.cpp
│   ├── ExcelProcessor.cpp
│   └── SpinnerWidget.cpp
└── resources/
    └── AppIcon.iconset/
```

---

## Licencia

MIT — libre para uso personal y comercial.
