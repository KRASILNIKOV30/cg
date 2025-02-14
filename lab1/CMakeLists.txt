add_executable(initials
        signals/SignallingValue.h
        initials/Model.h
        initials/main.cpp
        initials/View.h
        initials/LetterView.h
)

set_target_properties(initials PROPERTIES LINKER_LANGUAGE CXX)

find_package(wxWidgets REQUIRED COMPONENTS core base)
if (wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif ()

target_link_libraries(initials PRIVATE ${wxWidgets_LIBRARIES})