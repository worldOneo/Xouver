# CMakeList.txt: CMake-Projekt f�r "XouverRT". Schlie�en Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

include_directories ("${CMAKE_SOURCE_DIR}/XNI")

# F�gen Sie der ausf�hrbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (Xouver "XouverKit.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xouver PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries (Xouver xouverrt)
# TODO: F�gen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.