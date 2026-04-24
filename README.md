# 🧩 Desbloqueador3000

Aplicación de escritorio para eliminar la protección de hojas y libros de archivos Excel (`.xlsx`).

Funciona arrastrando el archivo a la app — el resultado se genera listo para usar.

> ⚠️ **Importante:**  
> - Solo funciona con archivos sin contraseña de apertura  
> - No rompe cifrado AES ni protege archivos cifrados  
> - Úsalo únicamente sobre archivos de tu propiedad o con permiso

---

## 📦 Descarga

### 🍎 macOS
👉 [Descargar última versión (.dmg)](../../releases/latest)

- macOS 12 Monterey o superior
- Compatible con Apple Silicon (M1/M2/M3) e Intel

---

### 🪟 Windows
👉 [Descargar última versión (.zip)](../../releases/latest)

- Windows 10 / 11
- No requiere instalación (portable)

---

## ⚙️ Cómo funciona

1. Arrastra un archivo `.xlsx` a la zona superior de la aplicación
2. La app:
   - Descomprime el archivo Excel (ZIP interno)
   - Elimina nodos de protección en XML:
     - `<sheetProtection>`
     - `<workbookProtection>`
   - Reempaqueta el archivo limpio
3. El archivo procesado aparece listo para arrastrar o guardar

---

## 🛠️ Compilar desde código fuente

### 📌 Requisitos

- CMake ≥ 3.16
- Qt 6.x (Widgets + Concurrent)
- Compilador:
  - macOS: Clang (Xcode Command Line Tools)
  - Windows: MSVC 2022

---

## 🍎 macOS

```bash
# Instalar dependencias
brew install cmake qt6

# Clonar repo
git clone https://github.com/<tu-usuario>/Desbloqueador3000.git
cd Desbloqueador3000

# Configurar y compilar
cmake -B build-mac \
  -DCMAKE_PREFIX_PATH=$(brew --prefix qt6) \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build-mac

# Ejecutar app
open build-mac/Desbloqueador3000.app
```

🪟 Windows

## Requisitos

- Qt 6 (MSVC 2022 64-bit)
- Visual Studio Build Tools 2022
- CMake 3.16+
- Ninja (opcional pero recomendado)

Descarga Qt aquí:
https://www.qt.io/download-qt-installer  

Durante la instalación selecciona:
- Qt 6.x
- MSVC 2022 64-bit
- Qt Widgets
- Qt Concurrent

---

## ⚙️ Compilación en Windows

```powershell
git clone https://github.com/your-username/Desbloqueador3000.git
cd Desbloqueador3000

# Configurar proyecto (IMPORTANTE usar MSVC)
cmake -B build `
  -DCMAKE_PREFIX_PATH="C:\Qt\6.11.0\msvc2022_64"

# Compilar en Release
cmake --build build --config Release
```

---

## ▶️ Ejecutar

El ejecutable se genera en:

```
build\Release\Desbloqueador3000.exe
```

---

## 📦 Crear versión portable (recomendado)

Después de compilar:

```powershell
# Crear carpeta de distribución
mkdir dist\windows

# Copiar ejecutable
copy build\Release\Desbloqueador3000.exe dist\windows\

# Añadir dependencias de Qt (OBLIGATORIO)
C:\Qt\6.11.0\msvc2022_64\bin\windeployqt.exe dist\windows\Desbloqueador3000.exe

# Comprimir release final
Compress-Archive -Path dist\windows\* -DestinationPath dist\Desbloqueador3000-windows.zip
```

---

## ⚠️ Notas importantes

- Usa siempre **MSVC 2022**, no MinGW
- `windeployqt` es obligatorio para ejecutar en otros PCs
- Si falla Qt, revisa que `CMAKE_PREFIX_PATH` apunta a `msvc2022_64`
- El `.exe` solo NO es suficiente sin las DLLs de Qt

