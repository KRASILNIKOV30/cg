add_executable(alchemy
        alchemy/Element.h
        alchemy/main.cpp
        alchemy/Model.h
)

set_target_properties(alchemy PROPERTIES LINKER_LANGUAGE CXX)
