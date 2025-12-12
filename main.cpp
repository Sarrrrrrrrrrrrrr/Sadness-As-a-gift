#include <iostream>
#include <cstdlib>
#include <conio.h>
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

string cekPesanObj(address root, int x, int y) {
    if (root == NIL) return "";
    if (root->x == x && root->y == y) return root->pesan;

    if (x < root->x) return cekPesanObj(root->left, x, y);
    else if(x> root->x) return cekPesanObj(root->right, x, y);
    else {
        if (y < root->y) return cekPesanObj(root->left, x, y);
        else return cekPesanObj(root->right, x, y);
    }
}

void mulaiBermain(address root, int radiusPandang) {
    int x = 0, y = 0;
    bool masihBermain = true;
    char pilihanBermain;
    string pesanObj;
    do {
        system("cls");
        cout << "======== MINUS ========" << endl;
        cout << "Posisi: " << x << ", " << y << endl;
        cout << "P: Player, #: Tembok, *: Obeject" << endl << endl;

        gambarPeta(root, x, y, radiusPandang);
        
        cout << "\n>Pesan: " << pesanObj;
        pesanObj = ""; // reset pesan

        cout << "\n>Gunakan x untuk keluar\n";
        cout << ">Gunakan (w/a/s/d) untuk bergerak\n";
        cout << "input: ";
        pilihanBermain = static_cast<char>(_getch());
        
        int langkahX = x;
        int langkahY = y;
        switch (pilihanBermain)
        {
        case 'w':
            langkahY++;
            break;
        case 's':
            langkahY--;
            break;
        case 'a':
            langkahX--;
            break;
        case 'd':
            langkahX++;
            break;
        case 'x':
            masihBermain = false;
        default:
            break;
        }

        pesanObj = cekPesanObj(root, langkahX, langkahY);

        if (!apakahTembok(root, langkahX, langkahY)) {
            x = langkahX;
            y = langkahY;
        }
    } while (masihBermain);
}

void buatNodeTembok(address &root, int xAwal, int yAwal, int xAkhir, int yAkhir) {
    // jika tembok vertikal, xAwal dan yAwal merupakan titik paling bawah dari tembok
    // jika tembok Horizontal, xAwal dan yAwal merupakan titik paling kiri dari tembok

    for (int y = yAwal; y <= yAkhir; y++) {
        for (int x = xAwal; x <= xAkhir; x++) {
            root = insert(root, x, y, "Tembok", "AWAS! Ada tembok, tidak bisa lewat!", true);
        }
    }
}

void inputTembok(address &root) {
    int xAwal, xAkhir, yAwal, yAkhir;
    system("cls");
    cout << "=========================" << endl;
    cout << "    PEMBUATAN TEMBOK     " << endl;
    cout << "=========================" << endl;
    cout << "NOTE: jika tembok vertikal, xAwal dan yAwal merupakan titik paling bawah dari tembok" << endl;
    cout << "NOTE: jika tembok Horizontal, xAwal dan yAwal merupakan titik paling kiri dari tembok" << endl;
    cout << "Input xAwal: ";
    cin >> xAwal;
    cout << "Input yAwal: ";
    cin >> yAwal;
    cout << "Input xAkhir: ";
    cin >> xAkhir;
    cout << "Input yAkhir: ";
    cin >> yAkhir;

    buatNodeTembok(root, xAwal, yAwal, xAkhir, yAkhir);
}

void inputObject(address &root) {
    root = insert(root, 0, -2, "Bunga", "NIH BUNGA BUAT KAMU", false); 
}

void menuDeveloper(address &root) {
    int pililihanDeveloper;
    do {
        system("cls");
        cout << "=========================" << endl;
        cout << "      SANG DEVELOPER     " << endl;
        cout << "=========================" << endl;
        cout << "1. Tambahin tembok" << endl;
        cout << "2. Tambahin Object" << endl;
        cout << "3. Keluar" << endl;
        cout << "-------------------------" << endl;
        cout << "Pilih menu (1-3): ";
        cin >> pililihanDeveloper;
        switch (pililihanDeveloper)
        {
        case 1:
            inputTembok(root);
            break;
        case 2:    
            inputObject(root);
            break;
        default:
            break;
        }
    } while (pililihanDeveloper != 3);
}

void lihatSetting(address &root, int &radiusPandang) {
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
            menuDeveloper(root);
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
            lihatSetting(root, radiusPandang);
            break;
        case 3:
            cout << "Terimakasih telah bermain!" << endl;
            break;
        default:
            break;
        }
    } while (pilihanMenu != 3);
}