#include <iostream>
#include <cstdlib>
#define NIL NULL

using namespace std;

typedef struct Node *address;

struct Node {
    int x, y;
    string nama;
    bool dilewati;
    string pesan;
    address left, right;
};

address createNode(int objectX, int objectY, string namaObject, string pesanObject, bool statusDilewati) {
    address node = new Node;
    node->x = objectX;
    node->y = objectY;
    node->nama = namaObject;
    node->pesan = pesanObject;
    node->dilewati = statusDilewati;
    node->left = node->right = NIL;
    return node;
}

address insert(address root, int x, int y, string namaObj, string pesanObj, bool statusDilewati){
    if (root == NIL) {
        root = createNode(x, y, namaObj, pesanObj, statusDilewati);
        return root;
    }

    if (x < root->x) root->left = insert(root->left, x, y, namaObj, pesanObj, statusDilewati);
    else if (x > root->x) root->right = insert(root->right, x, y, namaObj, pesanObj, statusDilewati);
    else {
        if (y < root->y) root->left = insert(root->left, x, y, namaObj, pesanObj, statusDilewati);
        else root->right = insert(root->right, x, y, namaObj, pesanObj, statusDilewati);
    }
    return root;
}

bool apakahTembok(address root, int x, int y) {
    if (root == NIL) return false;
    if (root->x == x && root->y == y) return root->dilewati;

    if (x < root->x) return apakahTembok(root->left, x, y);
    else if(x > root->x) return apakahTembok(root->right, x, y);
    else {
        if (y < root->y) return apakahTembok(root->left, x, y);
        else return apakahTembok(root->right, x, y);
    }
}

bool apakahObject(address root, int posisiX, int posisiY) {
    if (root == NIL) return false;
    if (root->x == posisiX && root->y == posisiY) return true;

    if (posisiX < root->x) return apakahObject(root->left, posisiX, posisiY);
    else if(posisiX> root->x) return apakahObject(root->right, posisiX, posisiY);
    else {
        if (posisiY < root->y) return apakahObject(root->left, posisiX, posisiY);
        else return apakahObject(root->right, posisiX, posisiY);
    }
}

void gambarPeta(address root, int posisiX, int posisiY, int radiusPandang) {
    for (int y = posisiY + radiusPandang; y >= posisiY - radiusPandang; y--) {
        for (int x = posisiX - radiusPandang; x <= posisiX + radiusPandang; x++) {
            if (x == posisiX && y == posisiY) cout << "P ";
            else if (apakahTembok(root, x, y)) cout << "# ";
            else if (apakahObject(root, x, y)) cout << "* ";
            else cout << ". ";
        }
        cout << endl;
    }
}

void mulaiBermain(address root, int radiusPandang) {
    int x = 0, y = 0;
    bool masihBermain = true;
    char pilihanBermain;
    do {
        system("cls");
        cout << "======== MINUS ========" << endl;
        cout << "Posisi: " << x << ", " << y << endl;
        cout << "P: Player, #: Tembok, *: Obeject" << endl << endl;

        gambarPeta(root, x, y, radiusPandang);
        cout << "\n-Gunakan x untuk keluar\n";
        cout << "-Gunakan (w/a/s/d) untuk bergerak\n";
        cout << "input: ";
        cin >> pilihanBermain;
        switch (pilihanBermain)
        {
        case 'w':
            y++;
            break;
        case 's':
            y--;
            break;
        case 'a':
            x--;
            break;
        case 'd':
            x++;
            break;
        case 'x':
            masihBermain = false;
        default:
            break;
        }

        // fungsi yang mengecek tembok (jika benar tembok, maka player tidak jalan)
    } while (masihBermain);
}

void lihatSetting(int &radiusPandang) {
    int pilihanSetting;
    do {
        system("cls");
        cout << "=========================" << endl;
        cout << "         SETTING         " << endl;
        cout << "=========================" << endl;
        cout << "1. Mengubah jarak pandang. (Saat ini " << radiusPandang << " m)" << endl;
        cout << "2. Developer Mode" << endl;
        cout << "3. Keluar" << endl;
        cout << "-------------------------" << endl;
        cout << "Pilih menu (1-3): ";
        cin >> pilihanSetting;
        switch (pilihanSetting)
        {
        case 1:
            cout << "Masukkan radius yang diinginkan: ";
            cin >> radiusPandang;
            break;
        case 2:
            // masukDevMode();
            // comingsoon
            break;
        default:
            break;
        }
    } while (pilihanSetting != 3);
}

int main () {
    address root = NIL;
    int pilihanMenu;
    int radiusPandang = 5;

    root = insert(root, 2, 0, "Tembok Kanan", "AWAS! Ada tembok, tidak bisa lewat!", true);
    root = insert(root, -2, 0, "Tembok Kiri", "AWAS! Ada tembok, tidak bisa lewat!", true);
    root = insert(root, 0, 2, "Tembok Atas", "AWAS! Ada tembok, tidak bisa lewat!", true);
    root = insert(root, 0, -2, "Tembok Bawah", "AWAS! Ada tembok, tidak bisa lewat!", true);

    root = insert(root, 1, 1, "Bunga", "NIH BUNGA BUAT KAMU", false); 
    do {
        system("cls");
        cout << "=========================" << endl;
        cout << "      RENDERING GAME     " << endl;
        cout << "=========================" << endl;
        cout << "1. Mulai Bermain" << endl;
        cout << "2. Setting" << endl;
        cout << "3. Keluar" << endl;
        cout << "-------------------------" << endl;
        cout << "Pilih menu (1-3): ";
        cin >> pilihanMenu;
        switch (pilihanMenu)
        {
        case 1:
            mulaiBermain(root, radiusPandang);
            break;
        case 2:
            lihatSetting(radiusPandang);
            break;
        case 3:
            cout << "Terimakasih telah bermain!" << endl;
            break;
        default:
            break;
        }
    } while (pilihanMenu != 3);
}