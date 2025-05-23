cmake_minimum_required(VERSION 3.16)
project(FilmCatalog LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        SQL
        LinguistTools
)
qt_standard_project_setup()
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES ru)

set(HEADERS_FILES
    include/FilmCatalog.h
    include/dbprovider.h
    include/UserProvider.h
    include/DynamicResourceCreator.h
    include/AdminMenu.h
    include/Authorization.h
    include/UserCard.h
    include/UserSelfEdit.h
    include/CatalogDisplayItem.h
    include/DisplayCatalog.h
    include/CatalogItemAdder.h
)

set(PROJECT_SOURCES
    main.cpp
    FilmCatalog.cpp
    dbprovider.cpp
    UserProvider.cpp 
    DynamicResourceCreator.cpp
    AdminMenu.cpp
    Authorization.cpp
    UserCard.cpp
    UserSelfEdit.cpp
    CatalogDisplayItem.cpp
    DisplayCatalog.cpp
    CatalogItemAdder.cpp
)

set(UI_FILES
    ui/FilmCatalog.ui
    ui/AdminMenu.ui
    ui/Authorization.ui
    ui/UserCard.ui
    ui/UserSelfEdit.ui
    ui/CatalogDisplayItem.ui
    ui/DisplayCatalog.ui
    ui/CatalogItemAdder.ui
)

set(RESOURCES_FILES
    resources/catalog.qrc
)

set (TRANSLATION_FILES
    translation/Catalog_ru.ts
)

#[==[
Options:
add_compile_definitions(AUTHORIZATION) - Enable authorization to the app
add_compile_definitions(SOCIAL) - Enable social function (admin menu and selfedit)
add_compile_definitions(FILTER) - Enable filter feature
add_compile_definitions(ADVANCEDFILTER) - enable advanced filtering
add_compile_definitions(PAGES) - Enable part-loading list
add_compile_definitions(FAVORITE) - Enable favorite function
add_compile_definitions(BUCKET) - Enable bucket feature
]==] 

add_compile_definitions(AUTHORIZATION)
add_compile_definitions(SOCIAL)
add_compile_definitions(FILTER)
add_compile_definitions(ADVANCEDFILTER)
add_compile_definitions(PAGES)
add_compile_definitions(FAVORITE)

qt_add_executable(FilmCatalog ${PROJECT_SOURCES} ${HEADERS_FILES} ${RESOURCES_FILES} ${UI_FILES})

qt6_add_translations(FilmCatalog
    QM_FILES_OUTPUT_VARIABLE qm_files
    TS_FILES ${TRANSLATION_FILES}
)
install(FILES ${qm_files} DESTINATION ${CMAKE_BINARY_DIR}/translations)

message(STATUS "Translation files: ${qm_files}")
add_custom_command(TARGET FilmCatalog POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:FilmCatalog>/translations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${qm_files}
        $<TARGET_FILE_DIR:FilmCatalog>/translations
)

set_target_properties(FilmCatalog
    PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE ON
)

target_link_libraries(FilmCatalog
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
)
