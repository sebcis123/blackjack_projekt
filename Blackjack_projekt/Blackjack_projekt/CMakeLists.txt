# CMakeList.txt: projekt CMake dla elementu Blackjack_projekt, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (Blackjack_projekt "Blackjack_projekt.cpp" "Blackjack_projekt.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Blackjack_projekt PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
