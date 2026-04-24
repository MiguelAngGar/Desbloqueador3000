cmake_minimum_required(VERSION 3.16)
project(Desbloqueador3000 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

# ── macOS: generar .icns desde el iconset ──────────────────────────────────
if(APPLE)
    set(ICONSET_DIR "${CMAKE_SOURCE_DIR}/resources/AppIcon.iconset")
    set(ICNS_FILE   "${CMAKE_BINARY_DIR}/AppIcon.icns")

    add_custom_command(
        OUTPUT  "${ICNS_FILE}"
        COMMAND iconutil -c icns "${ICONSET_DIR}" -o "${ICNS_FILE}"
        DEPENDS "${ICONSET_DIR}"
        COMMENT "Generando AppIcon.icns"
    )
    add_custom_target(AppIconTarget DEPENDS "${ICNS_FILE}")
    set(APP_ICON_MACOS "${ICNS_FILE}")
endif()

# ── Sources ────────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/DropZone.cpp
    src/ResultPanel.cpp
    src/ExcelProcessor.cpp
    src/SpinnerWidget.cpp
)

set(HEADERS
    include/MainWindow.h
    include/DropZone.h
    include/ResultPanel.h
    include/ExcelProcessor.h
    include/SpinnerWidget.h
    include/Utils.h
)

# ── Executable ────────────────────────────────────────────────────────────
qt_add_executable(Desbloqueador3000
    ${SOURCES}
    ${HEADERS}
    ${APP_ICON_MACOS}
)

if(APPLE)
    add_dependencies(Desbloqueador3000 AppIconTarget)
    set_source_files_properties("${ICNS_FILE}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
endif()

target_include_directories(Desbloqueador3000 PRIVATE include)
target_link_libraries(Desbloqueador3000 PRIVATE Qt6::Widgets Qt6::Concurrent)

# ── macOS bundle ──────────────────────────────────────────────────────────
if(APPLE)
    set_target_properties(Desbloqueador3000 PROPERTIES
        MACOSX_BUNDLE                   TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER    io.github.desbloqueador3000
        MACOSX_BUNDLE_BUNDLE_NAME       "Desbloqueador3000"
        MACOSX_BUNDLE_BUNDLE_VERSION    ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE         AppIcon.icns
    )
endif()

# ── Windows ───────────────────────────────────────────────────────────────
if(WIN32)
    set_target_properties(Desbloqueador3000 PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
