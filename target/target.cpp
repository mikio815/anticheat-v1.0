// anticheat.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <conio.h>
#include <windows.h>

int ammo = 10;
const int max_ammo = 10;

void print_ammo() {
    std::cout << "Current ammo: " << ammo << std::endl;
}

int main() {
    std::cout << "Pless ENTER to shoot, Pless R to reload, Press ESC to quit." << std::endl;

    while (true) {
        if (_kbhit()) {
            int key = _getch();

            if (key == 27) {
                std::cout << "Exiting." << std::endl;
                break;
            }

            if (key == '\r') {
                if (ammo > 0) {
                    ammo--;
                    std::cout << "Bang!" << std::endl;
                }
                else {
                    std::cout << "Out of ammo!" << std::endl;
                    std::cout << "Pless R to reload.\n" << std::flush;
                }
                print_ammo();
            }

            if (key == 'r' || key == 'R') {
                ammo = max_ammo;
                std::cout << "Reloaded!\n" << std::flush;
                print_ammo();
            }
        }

        Sleep(10);
    }

    return 0;
}
