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
)
qt_standard_project_setup()

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

qt_add_executable(FilmCatalog ${PROJECT_SOURCES} ${HEADERS_FILES} ${RESOURCES_FILES} ${UI_FILES})

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
