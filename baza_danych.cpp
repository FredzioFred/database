#include <iostream>
#include <fstream>
#include <filesystem>
#include <conio.h>
#include <vector>

class BazaDanach
{
private:
    std::string mNazwaFolderuTabeli = "tables";

public:
    void utworzTabele()
    {
        std::string nazwaTabeli;

        std::cout << "Podaj nazwe tabeli: ";
        std::cin >> nazwaTabeli;
        std::filesystem::path directorypath = std::filesystem::path(mNazwaFolderuTabeli) / nazwaTabeli;
        if (!exists(directorypath))
        {
            if (create_directory(directorypath) == true)
            {
                std::cout << "Tabela zostala utworzona pomyslnie!\n";
            }
            else
            {
                std::cout << "Tabela nie moze zostac utworzona!\n";
            }
        }
        else
        {
            std::cout << "Tabela juz istnieje!\n";
        }
    }

    void usunTabele()
    {
        std::cout << "Wybierz tabele: \n";
        int licznik = 0;
        int wybranaOpcja = 0;
        for (const auto &entry : std::filesystem::directory_iterator(mNazwaFolderuTabeli))
        {
            std::cout << "[" << licznik << "] " << entry.path().filename() << std::endl;
            licznik++;
        }

        std::cin >> wybranaOpcja;
        std::filesystem::path folderDoUsuniecia;
        int licznik1 = 0;
        for (const auto &entry : std::filesystem::directory_iterator(mNazwaFolderuTabeli))
        {
            if (licznik1 == wybranaOpcja)
            {
                folderDoUsuniecia = entry.path();
                std::cout << entry << std::endl;
                std::cout << "Czy na pewno chcesz usunac tabele : " << entry.path().filename() << " ? (0 - Nie, 1 - Tak) \n";
                int wybor = 0;
                std::cin >> wybor;
                if (wybor == 1)
                {
                    std::cout << "Tabela zostala usunieta!\n";
                    break;
                }
                else if (wybor == 0)
                {
                    std::cout << "Tabela nie zostala usunieta!\n";
                }
                else
                {
                    std::cout << "Niepoprawny wybor\n";
                }
            }
            licznik1++;
        }

        std::error_code ec;

        std::filesystem::permissions(
            folderDoUsuniecia,
            std::filesystem::perms::owner_all,
            std::filesystem::perm_options::add);

        std::filesystem::remove_all(folderDoUsuniecia, ec);

        if (ec)
        {
            std::cout << "Nie mozna usunac: "
                      << folderDoUsuniecia
                      << " | blad: "
                      << ec.message()
                      << '\n';
            ec.clear();
        }
    }

    void wypiszTabele()
    {
        int licznik = 0;
        for (const auto &entry : std::filesystem::directory_iterator(mNazwaFolderuTabeli))
        {
            std::cout << "[" << licznik << "] " << entry.path().filename() << std::endl;
            licznik++;
        }

        std::cout << "Wcisnij enter aby kontynuowac!\n";
        getch();
    }

    std::filesystem::path wybierzTabeleDoModyfikacji()
    {
        std::cout << "Ktorej tabeli chesz nadac wlasciowosci?\n";

        int licznik = 0;
        std::filesystem::path tabelaDoModyfikacji;

        for (const auto &entry : std::filesystem::directory_iterator(mNazwaFolderuTabeli))
        {
            std::cout << "[" << licznik << "] " << entry.path().filename() << std::endl;
            licznik++;
        }

        int wybor = 0;
        std::cin >> wybor;

        int licznik2 = 0;

        for (const auto &entry : std::filesystem::directory_iterator(mNazwaFolderuTabeli))
        {
            if (licznik2 == wybor)
            {
                std::cout << entry.path().filename() << std::endl
                          << std::endl;
                tabelaDoModyfikacji = entry;
                break;
            }

            licznik2++;
        }

        std::cout << tabelaDoModyfikacji << std::endl;

        return tabelaDoModyfikacji;
    }

    void utworzSchemat(std::filesystem::path tabelaDoModyfikacji)
    {
        std::filesystem::path schemat = "schemat.txt";
        std::filesystem::path pelna_sciezka_schematu = tabelaDoModyfikacji / schemat;
        ;
        std::ofstream schema(pelna_sciezka_schematu);

        int iloscKolumn = 0;
        std::cout << "Ile mam byc kolumn : ";
        std::cin >> iloscKolumn;

        for (int i = 0; i < iloscKolumn; i++)
        {
            std::cout << "Jak ma nazywac sie " << i << " kolumna? ";

            std::string nazwa;
            std::cin >> nazwa;

            schema << nazwa << std::endl;
        }
    }

    void nadajWlasciwosciTabeli()
    {
        std::filesystem::path tabelaDoModyfikacji = wybierzTabeleDoModyfikacji();

        utworzSchemat(tabelaDoModyfikacji);
        std::cout << "\n";
    }

    void podajWierszDoTabeli()
    {
        std::filesystem::path tabelaDoModyfikacji;
        tabelaDoModyfikacji = wybierzTabeleDoModyfikacji();

        std::filesystem::path wartosc = "dane.txt";
        std::filesystem::path pelna_sciezka_danych = tabelaDoModyfikacji / wartosc;

        std::ofstream dane(pelna_sciezka_danych, std::ios::app);

        std::filesystem::path schemat = "schemat.txt";
        std::filesystem::path pelna_sciezka_schematu = tabelaDoModyfikacji / schemat;
        ;
        std::ifstream schema(pelna_sciezka_schematu);

        std::cin.ignore();
        while (schema)
        { // tutaj
            std::string nazwa;

            schema >> nazwa;
            if (nazwa == "")
            {
                break;
            }
            std::cout << "Nadaj wartosc " << nazwa << " : ";
            std::string wartoscPodanaOdUzytkownika;
            std::getline(std::cin, wartoscPodanaOdUzytkownika);
            dane << wartoscPodanaOdUzytkownika << ",";
        }
        dane << std::endl;
    }

    void wypiszDaneTabeli()
    {
        std::filesystem::path tabelaDoModyfikacji = wybierzTabeleDoModyfikacji();

        std::filesystem::path wartosc = "schemat.txt";
        std::filesystem::path pelna_sciezka_schemat = tabelaDoModyfikacji / wartosc;
        std::ifstream schemat(pelna_sciezka_schemat);

        std::filesystem::path wartoscDane = "dane.txt";
        std::filesystem::path pelna_sciezka_dane = tabelaDoModyfikacji / wartoscDane;
        std::ifstream dane(pelna_sciezka_dane);

        std::vector<std::string> kolumny;

        while (schemat)
        {
            std::string temp;
            schemat >> temp;
            kolumny.push_back(temp);
        }

        std::string calyWiersz;

        std::vector<std::vector<std::string>> kolumnyIDane;

        kolumnyIDane.push_back(kolumny);

        while (getline(dane, calyWiersz))
        {
            std::vector<std::string> tymczasowy;
            for (int i = 0; i < kolumny.size(); i++)
            {
                std::size_t found = calyWiersz.find(",");
                std::string temp = calyWiersz.substr(0, found);
                tymczasowy.push_back(temp);
                calyWiersz = calyWiersz.substr(found + 1, calyWiersz.size() - 1);
            }

            kolumnyIDane.push_back(tymczasowy);
        }

        std::vector<int> najwiekszyElement(kolumny.size(), 0);

        for (int i = 0; i < kolumnyIDane.size(); i++)
        {
            for (int j = 0; j < kolumnyIDane[i].size(); j++)
            {
                int dlugosc;
                dlugosc = kolumnyIDane[i][j].size();
                if (dlugosc > najwiekszyElement[j])
                {
                    najwiekszyElement[j] = dlugosc;
                }
            }
        }

        bool pierwszaLinia = true;
        int liniaRozmiar = 0;
        std::string linia = "";
        for (int i = 0; i < najwiekszyElement.size(); i++)
        {
            liniaRozmiar += najwiekszyElement[i];
            liniaRozmiar += 2;
        }

        for (int i = 0; i < liniaRozmiar-4; i++)
        {
            linia.push_back('-');
        }

        for (int i = 0; i < kolumnyIDane.size(); i++)
        {
            for (int j = 0; j < kolumnyIDane[i].size(); j++)
            {
                std::cout << kolumnyIDane[i][j];
                const int ustalonaIlosSpacji = 2;
                int wynik = 0;
                wynik = najwiekszyElement[j] - kolumnyIDane[i][j].size();
                wynik += ustalonaIlosSpacji;
                for (int k = 0; k < wynik; k++)
                {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
            if (pierwszaLinia == true)
            {
                std::cout << linia << std::endl;
                pierwszaLinia = false;
            }
        }

        std::cout << "Wcisnij enter aby kontynuowac!\n";
        getch();
    }
};
int wypiszMenuGlowne()
{
    int wybranaOpcja = 0;
    std::cout << R"(______                      _                        _     
| ___ \                    | |                      | |
| |_/ / __ _ ______ _    __| | __ _ _ __  _   _  ___| |__  
| ___ \/ _` |_  / _` |  / _` |/ _` | '_ \| | | |/ __| '_ \ 
| |_/ / (_| |/ / (_| | | (_| | (_| | | | | |_| | (__| | | |
\____/ \__,_/___\__,_|  \__,_|\__,_|_| |_|\__, |\___|_| |_|
                                           __/ |           
                                          |___/            
    )";

    std::cout << "Wybierz co chcesz zrobic:\n";
    std::cout << "[1] Utworz pusta tabele\n";
    std::cout << "[2] Usun tabele\n";
    std::cout << "[3] Pokaz tabele\n";
    std::cout << "[4] Nadaj wlasciwosci tabeli\n";
    std::cout << "[5] Podaj wiersz do tabeli\n";
    std::cout << "[6] Wyswietl wartosci tabeli\n";
    std::cout << "[7] Zamknij program\n";
    std::cin >> wybranaOpcja;
    return wybranaOpcja;
}
int main()
{
    BazaDanach bazadanych;
    while (true)
    {
        switch (wypiszMenuGlowne())
        {
        case 1:
            bazadanych.utworzTabele();
            break;

        case 2:
            bazadanych.usunTabele();
            break;

        case 3:
            bazadanych.wypiszTabele();
            break;

        case 4:
            bazadanych.nadajWlasciwosciTabeli();
            break;

        case 5:
            bazadanych.podajWierszDoTabeli();
            break;

        case 6:
            bazadanych.wypiszDaneTabeli();
            break;

        default:
            exit(0);
            break;
        }
    }
    return 0;
}