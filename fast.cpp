/*
 *Nom du fichier: 327317.cpp
 *Auteur        : Christian Matala
 *Description   : Projet de semestre d'automne 2020 FAST VERSION (VERY FAST)
*/
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const string P1("P1 missing or incorrect");
const string WRONG_NB_COL_LGN("Nb columns and/or nb lines incorrect");
const string MATRIX_SQUARE("Matrix is not square");
const string IRREGULAR_CONTENT("Matrix description with incorrect content");
const string DISCONNECTED_GRAPH("Matrix corresponds to a disconnected graph");

typedef vector<vector<unsigned int>> Matrix;

Matrix transform_adj_matrix(vector<vector<unsigned int>>& mat_adj, unsigned int& n);

unsigned int check_and_return_matrix_size(){
    string format;
    cin>>format;    
    if(format!="P1"){
        cout << P1 << endl;
        exit(0);
    }
    
    unsigned int c;
    unsigned int l;
    cin>>c>>l;
	if(c!=l){
        cout << MATRIX_SQUARE << endl;
        exit(0);
    }
	if(c==0 || l==0){
        cout << WRONG_NB_COL_LGN << endl;
        exit(0);
    }
    
    return l;
}

Matrix create_matrix(Matrix& voisins_noeud, unsigned int& n){
	vector<bool> visited(n);
    for(unsigned int i(0); i<n; i++){
        string line;
        getline(cin>>ws, line);
        unsigned int line_size(line.size());        
        if(line_size != (2*n-1)){
            cout << WRONG_NB_COL_LGN << endl;
            exit(0);
        }
        
		while( line.find("1") <=line.size()){
			int indice(line.find("1")/2);
			voisins_noeud[i].push_back(indice);
            voisins_noeud[indice].push_back(i);
			line[2*indice] = 0;
			if(visited[i]==false) visited[i]=true;
			if(visited[indice]==false) visited[indice]=true;
		}
    }
    vector<bool>true_table(n, true);
    if(visited != true_table){
        cout<<DISCONNECTED_GRAPH<<endl;
        exit(0);
    }
    return voisins_noeud;
}
void propagation(Matrix& voisins_noeud, unsigned int& n, vector<bool>& visited, vector<vector<unsigned int>>& degre_propagation_table, vector<unsigned int> depart){
	degre_propagation_table.push_back(depart);
	for(auto element:depart){
		visited[element]=true;
	}
    
    vector<unsigned int> arrivee;
	for(unsigned int i(0); i<depart.size(); i++){
		unsigned int noeud(depart[i]);
		for(unsigned int j(0); j<voisins_noeud[noeud].size(); j++){
			unsigned int voisin = voisins_noeud[noeud][j];
            if(visited[voisin]==false){
				visited[voisin]=true;
				arrivee.push_back(voisin);
			}
		}
	}
	if(!(arrivee.empty())){
        //sort(arrivee.begin(), arrivee.end());
        propagation(voisins_noeud, n, visited, degre_propagation_table, arrivee);
    }
}

double degre_propagation_moyen_noeud(Matrix& voisins_noeud, unsigned int& n, unsigned int noeud, bool display){
	vector<bool> visited(n);
	vector<vector<unsigned int>> degre_propagation_table;
    propagation(voisins_noeud, n, visited, degre_propagation_table, {noeud});
    unsigned int m(degre_propagation_table.size());
    
    if(display){
        for(unsigned int i(0); i<m; i++){
            unsigned int size(degre_propagation_table[i].size());
            for(unsigned int j(0); j<size; j++){
                cout << degre_propagation_table[i][j];
                if(j<size-1) cout << " ";
            }
            cout << endl;
        }
    }
    
    double somme_degre(0);
    for(unsigned int j(1); j<m; j++){
		unsigned int nb_noeuds(degre_propagation_table[j].size());
        somme_degre+=j*nb_noeuds;
    }

    switch (n){
        case 1 : return 0;
        default: return somme_degre/(n-1);
    } 
}

double degre_separation_moyen(Matrix& voisins_noeud, unsigned int& n){
    double somme_degre_moyen(0);
    for(unsigned int i(0); i<n; i++){
        switch(i){
            case 0:     somme_degre_moyen += degre_propagation_moyen_noeud(voisins_noeud, n, i, true ); break;
            default:    somme_degre_moyen += degre_propagation_moyen_noeud(voisins_noeud, n, i, false); break;
        }
    }
    return (somme_degre_moyen/n);
}

void display_degre_moyen_graphe(Matrix& voisins_noeud, unsigned int& n){
    double degre_moyen_graphe(degre_separation_moyen(voisins_noeud, n));
    cout << setprecision(6);
    cout << fixed;
    cout << degre_moyen_graphe;
    cout << endl;
}

int main(){
    unsigned int n(check_and_return_matrix_size());
    Matrix voisins_noeud(n);
    create_matrix(voisins_noeud, n);
    display_degre_moyen_graphe(voisins_noeud, n);
}