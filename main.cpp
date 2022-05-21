#include <bits/stdc++.h>
using namespace std;
#define V vector
#define mat V<V<int>>
#include <unordered_set>

struct implicant {
    string name;
    int size;
    bool isEssential;
    implicant(string n, int s) {
        this->name = n;
        this->size = s;
        isEssential = false;
    }
    implicant() {
        this->name = "";
        this->size = 0;
        isEssential = false;
    }
};

bool compareByLength(const implicant& a, const implicant& b) {
    
    return (a.size > b.size);
}

void printMap(mat& maap) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++)
            cout << maap[i][j] << " ";
        cout << "\n";
    }
}

set<pair<int, int>> inters(set<pair<int, int>>& count_1, set<pair<int, int>>& count_2) {
    set<pair<int, int>> ans;
    for (auto u : count_1) if (count_2.count(u)) ans.emplace(u);
    return ans;
}

set<pair<int, int>> convert(string implic) {
    set<pair<int, int>> ans;
    for (int i = 0; i < 2; i++) for (int j = 0; j < 4; j++) ans.emplace(i, j);
    reverse(implic.begin(), implic.end());
    while (!implic.empty()) {
        set<pair<int, int>> temp;
        char c = implic.back(); implic.pop_back();
        bool is_bar = (!implic.empty() && implic.back() == '\'');
        if (is_bar) implic.pop_back();
        if (c == 'A')
            for (int j = 0; j < 4; j++) temp.emplace(!is_bar, j);
        else {
            pair<int, int> corner = { 0, 0 };
            corner.second = (6 - (is_bar ? 2 : 0) + 'B' - c) % 4;
            temp.emplace(corner);
            temp.emplace(corner.first, (corner.second + 1) % 4);
            temp.emplace(corner.first + 1, (corner.second + 1) % 4);
            temp.emplace(corner.first + 1, corner.second);
        }
        ans = inters(ans, temp);
    }
    return ans;
}

int gfh(int idx) {
    if (idx == 2) return 3;
    return (idx == 3 ? 2 : idx);
}

void minimize_permutation(int& running_min, vector<string>& ans_, vector<string>& cur) {
    vector<string> tempo;
    vector<vector<bool>> vis(2, vector<bool>(4));
    int count = 0;
    for (auto& implic : cur) {
        set<pair<int, int>> pairs = convert(implic);
        bool is_all_vis = true;
        for (auto p : pairs) if (!vis[p.first][p.second]) {
            is_all_vis = false;
            vis[p.first][p.second] = 1;
        }
        if (is_all_vis) continue;
        tempo.emplace_back(implic);
        count++;
    }
    if (count < running_min) {
        ans_ = tempo;
        running_min = count;
    }
}

int gfh2(int idx) {
    if (idx == 6) return 7;
    return (idx == 7 ? 6 : idx);
}
int main() {
    kmap:
        vector<int> minterms;
        imp:
        cout << "Enter the Number of Terms: ";
        int n; cin >> n;
        if (n < 0 || n > 7)
        {
            cout << "==============================" << endl;
            goto imp;
        }
        int m = 0;
        cout << "Enter the Terms: ";
        while (n > 0) {
            term:
            cin >> m;
            if(m < 0 || m > 7)
            {
                cout << "Invalid." << endl;
                goto term;
            }
            minterms.push_back(m);
            n--;
        }
        if ((int)minterms.size() == 8) {
            cout << 1 << "\n";
        }
        mat maap(2, V<int>(5, 0));
        for (int i = 0; i < minterms.size(); i++) {
            int row = minterms[i] / 4;
            int col = minterms[i] % 4;
            if (col == 3)
                col = 2;
            else if (col == 2)
                col = 3;
            maap[row][col] = 1;
        }
        vector<implicant> cells[8];

        cout << "==============================" << endl;
        cout << "The Generated kmap: " << endl;
        cout << "==============================" << endl;
        printMap(maap);
        cout << "==============================" << endl;

        //horizontal search
        vector<string> imp;

        for (int i = 0; i < 2; i++) {
            string ins;
            if (i == 0)
                ins += "0";
            else
                ins += "1";
            for (int j = 0; j < 5; j++) {
                if (maap[i][j] == 1)
                    ins += to_string(gfh(j));
                else if (j != 0 && maap[i][j - 1] == 1) {
                    int end = 1 << (j / 2);
                    string one, two;
                    if (ins.size() == 4) {
                        one = ins.substr(0, 3);
                        two = ins.substr(0, 1);
                        two += ins.substr(2, 2);
                        imp.push_back(one);
                        imp.push_back(two);
                    }
                    else {
                        imp.push_back(ins);
                    }

                    ins.clear();
                    if (j != 5)
                        ins += to_string(i);
                }
            }
            if (maap[i][0] == maap[i][3] && maap[i][0] == 1 && (maap[i][2] != 1 || maap[i][1] != 1))
                imp.push_back(to_string(i) + "02");
        }
        vector<implicant> lett;
        map<string, implicant> hmap;
        for (int i = 0; i < imp.size(); i++) {
            if (imp[i].size() == 5) {
                if (imp[i][0] == '0') {
                    string name = "A'";
                    lett.push_back(implicant(name, 4));
                    hmap[imp[i]] = (implicant(name, 4));
                }
                else {
                    string name = "A";
                    lett.push_back(implicant(name, 4));
                    hmap[imp[i]] = implicant(name, 4);
                }
            }
            else if (imp[i].size() == 3) {
                if (imp[i][0] == '0') {
                    string name = "A'";
                    if ((imp[i][1] == '0' && imp[i][2] == '1') || (imp[i][2] == '0' && imp[i][2] == '1')) {
                        name += "B'";
                    }
                    if ((imp[i][1] == '1' && imp[i][2] == '2') || (imp[i][2] == '1' && imp[i][1] == '2') || (imp[i][1] == '1' && imp[i][2] == '3') || (imp[i][2] == '1' && imp[i][1] == '3')) {
                        name += "C";
                    }
                    if ((imp[i][1] == '2' && imp[i][2] == '3') || (imp[i][2] == '2' && imp[i][1] == '3')) {
                        name += "B";
                    }
                    if ((imp[i][1] == '0' && imp[i][2] == '2') || (imp[i][2] == '0' && imp[i][1] == '2')) {
                        name += "C'";
                    }
                    lett.push_back(implicant(name, 2));
                    hmap[imp[i]] = implicant(name, 2);
                }

                else if (imp[i][0] == '1') {
                    string name = "A";
                    if ((imp[i][1] == '0' && imp[i][2] == '1') || (imp[i][2] == '0' && imp[i][1] == '1')) {
                        name += "B'";
                    }
                    if ((imp[i][1] == '1' && imp[i][2] == '3') || (imp[i][2] == '1' && imp[i][1] == '3')) {
                        name += "C";
                    }
                    if ((imp[i][1] == '2' && imp[i][2] == '3') || (imp[i][2] == '2' && imp[i][1] == '3')) {
                        name += "B";
                    }
                    if ((imp[i][1] == '0' && imp[i][2] == '2') || (imp[i][2] == '0' && imp[i][1] == '2')) {
                        name += "C'";
                    }
                    lett.push_back(implicant(name, 2));
                    hmap[imp[i]] = implicant(name, 2);
                }
            }

            else if (imp[i].size() == 2) {
                if (imp[i][0] == '0') {
                    string name = "A'";
                    if (imp[i][1] == '0')
                        name += "B'C'";
                    else if (imp[i][1] == '1')
                        name += "B'C";
                    else if (imp[i][1] == '3')
                        name += "BC";
                    else if (imp[i][1] == '2')
                        name += "BC'";
                    if (maap[1][gfh(imp[i][1] - '0')] == 0) {
                        lett.push_back(implicant(name, 1));
                        hmap[imp[i]] = implicant(name, 1);
                    }
                }
                else if (imp[i][0] == '1') {
                    string name = "A";
                    if (imp[i][1] == '0')
                        name += "B'C'";
                    else if (imp[i][1] == '1')
                        name += "B'C";
                    else if (imp[i][1] == '3')
                        name += "BC";
                    else if (imp[i][1] == '2')
                        name += "BC'";
                    if (maap[0][gfh(imp[i][1] - '0')] == 0) {
                        lett.push_back(implicant(name, 1));
                        hmap[imp[i]] = implicant(name, 1);
                    }
                }

            }
        }

        for (int i = 0; i < imp.size(); i++) {
            for (int j = 1; j < imp[i].size(); j++) {
                int r = 4 * (imp[i][0] - '0');
                r += (imp[i][j] - '0');
                implicant tmp = hmap[imp[i]];
                if (tmp.name != "")
                    cells[r].push_back(hmap[imp[i]]);

            }
        }
        vector<implicant> tmp = cells[2];
        cells[2] = cells[3];
        cells[3] = tmp;
        tmp = cells[6];
        cells[6] = cells[7];
        cells[7] = tmp;


        //vertical search
        for (int i = 0; i < 4; i++) {
            if (maap[0][i] == 1 && maap[1][i] == 1) {
                if (i == 0) {
                    cells[0].push_back(implicant("B'C'", 2));
                    cells[i + 4].push_back(implicant("B'C'", 2));
                }
                else if (i == 1) {
                    cells[i].push_back(implicant("B'C", 2));
                    cells[i + 4].push_back(implicant("B'C", 2));
                }
                else if (i == 2) {
                    cells[2].push_back(implicant("BC", 2));
                    cells[6].push_back(implicant("BC", 2));
                }
                else if (i == 3) {
                    cells[3].push_back(implicant("BC'", 2));
                    cells[7].push_back(implicant("BC'", 2));
                }
            }
        }
        for (int i = 1; i < 5; i++) {
            int in = i % 4;
            if (maap[0][i - 1] == 1 && maap[1][i - 1] == 1 && maap[0][in] == 1 && maap[1][in] == 1) {
                if (i - 1 == 0) {
                    cells[0].push_back(implicant("B'", 4));
                    cells[1].push_back(implicant("B'", 4));
                    cells[4].push_back(implicant("B'", 4));
                    cells[5].push_back(implicant("B'", 4));
                }
                else if (i - 1 == 1) {
                    cells[1].push_back(implicant("C", 4));
                    cells[2].push_back(implicant("C", 4));
                    cells[5].push_back(implicant("C", 4));
                    cells[6].push_back(implicant("C", 4));
                }
                else if (i - 1 == 2) {
                    cells[3].push_back(implicant("B", 4));
                    cells[2].push_back(implicant("B", 4));
                    cells[7].push_back(implicant("B", 4));
                    cells[6].push_back(implicant("B", 4));
                }
                else if (i - 1 == 3) {
                    cells[0].push_back(implicant("C'", 4));
                    cells[4].push_back(implicant("C'", 4));
                    cells[3].push_back(implicant("C'", 4));
                    cells[7].push_back(implicant("C'", 4));
                }
            }
        }

        // computing the anwser
        cout << "The boolean expression: " << endl;
        cout << "==============================" << endl;
        cout << "F = ";

        int count = 0;
        unordered_set<string> eq;
        vector<string> ess;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (maap[i][j] == 1)
                {
                    vector<implicant> im = cells[count];
                    if (im.size() == 1) {
                        ess.push_back(im[0].name);
                    }
                }
                count++;
            }
        }

        count = 0;
        map<string, int> to_size;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (maap[i][j] == 1)
                {
                    vector<implicant> im = cells[count];
                    //sort(im.begin(), im.end(), compareByLength);
                    if (!im.empty()) {
                        for (auto i : im)
                            eq.insert(i.name), to_size[i.name] = i.size;
                    }
                }
                count++;
            }
        }

        vector<string> cur;
        for (auto& implic : eq) cur.emplace_back(implic);
        vector<string> ans_ = cur;
        int running_min = cur.size();
        sort(cur.begin(), cur.end(), [&](string& str1, string& str2) { return to_size[str1] > to_size[str2];  });
        minimize_permutation(running_min, ans_, cur);
        cur = ans_;
        sort(cur.begin(), cur.end());
        do {
            minimize_permutation(running_min, ans_, cur);
        } while (next_permutation(cur.begin(), cur.end()));
        int ss = 0;
        bool fg = true;
        for (auto& itr : ans_) {
            if (ss == ans_.size() - 1)
            {
                cout << itr;
                fg = false;
            }
            else
            {
                cout << itr << " + ";
                fg = false;
            }
            ss++;
        }
        if (fg) cout << 0;
        cout << "\n";
        cout << "==============================" << endl;
        cout << "Again? (y or n) ";
        try {
            char b;
            cin >> b;
            if (b == 'y' or b == 'Y')
            {
                cout << "==============================" << endl;
                goto kmap;
            }
            else {
                cout << "==============================" << endl;
                cout << "Thanks." << endl;
                cout << "==============================" << endl;
                return 0;
            }
            throw b;
        }
        catch (char b) {
            cout << "==============================" << endl;
            cout << "Thanks." << endl;
            cout << "==============================" << endl;
            return 0;
        }
}