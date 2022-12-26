# CMakeList.txt: CMake-Projekt für "XouverRT". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (Xouver "XouverKit.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xouver PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries (Xouver xouverrt)
# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.