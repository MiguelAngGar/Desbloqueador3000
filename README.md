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
