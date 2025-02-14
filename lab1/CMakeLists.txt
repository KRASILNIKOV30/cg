add_executable(initials
        signals/SignallingValue.h
        initials/Model.h
        initials/main.cpp
        initials/View.h
        initials/LetterView.h
)

add_executable(kamaz
        signals/SignallingValue.h
        kamaz/main.cpp
        kamaz/Model.h
        kamaz/View.h
        kamaz/Controller.h
)

add_executable(circle
        circle/main.cpp
        circle/Canvas.h
        circle/Drawing.h
)

set_target_properties(initials PROPERTIES LINKER_LANGUAGE CXX)

find_package(wxWidgets REQUIRED COMPONENTS core base)
if (wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif ()

target_link_libraries(initials PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(kamaz PUBLIC ${wxWidgets_LIBRARIES})
target_link_libraries(kamaz PUBLIC ${wxWidgets_LIBRARIES})
target_link_libraries(circle PUBLIC ${wxWidgets_LIBRARIES})