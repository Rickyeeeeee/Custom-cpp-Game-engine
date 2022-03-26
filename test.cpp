#include <vector>
#include <string>
#include <iostream>

using namespace std;



void multiply(float** A, float** B, float** C, float** D, float** E, float** F, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            F[i][j] = 0;
            E[i][j] = 0;
        }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < m; k++) {
                E[i][j] += A[i][m] * C[m][j] - B[i][m] * D[m][j];
                F[i][j] += A[i][m] * D[m][j] + B[i][m] * C[m][j];
            }
}

int main()
{
    std::string a = "jl";
    std::string b(a.size(), 's');
    b[0] = 's';
    cout << b[0] << endl;

}