add_executable(
        initials

        main.cpp
        model/Letter.cpp
        model/Letters.cpp
        view/LetterView.cpp
        controller/LetterController.cpp
)

target_link_libraries(initials sfml-graphics sfml-window sfml-system)