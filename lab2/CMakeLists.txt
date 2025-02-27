add_executable(alchemy
        alchemy/Element.h
        alchemy/main.cpp
        alchemy/Model.h
        alchemy/View.h
        alchemy/Controller.h
)

set_target_properties(alchemy PROPERTIES LINKER_LANGUAGE CXX)

find_package(wxWidgets REQUIRED COMPONENTS core base)
if (wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif ()

target_link_libraries(alchemy PRIVATE ${wxWidgets_LIBRARIES})