// Fuente: https://medium.com/@shashikantsolanki042/strassens-matrix-multiplication-algorithm-b6cc3b2287df

#include <bits/stdc++.h>
using namespace std;
//Function to print the Matrix.
void Print(vector<vector<int>>A)
{
 for(int i=0; i<A.size(); i++)
 {
 for(int j=0; j<A.size(); j++)
 {
 cout << A[i][j] << " ";
 }
 cout << "\n";
 }
 cout << "\n";
}
//Function to Add to Matrices.
vector<vector<int>> add(vector<vector<int>>A, vector<vector<int>>B, int size)
{
 vector<vector<int>>C(size,vector<int>(size,0));
 for(int i=0; i<size; i++)
 {
 for(int j=0; j<size; j++)
 {
 C[i][j] = A[i][j] + B[i][j];
 }
 }
 return C;
}
//Function to subtract two matrices.
vector<vector<int>> subtract(vector<vector<int>>A, vector<vector<int>>B, int size)
{
 vector<vector<int>>C(size,vector<int>(size,0));
 for(int i=0; i<size; i++)
 {
 for(int j=0; j<size; j++)
 {
 C[i][j] = A[i][j] - B[i][j];
 }
 }
 return C;
}
//Function to perform stressen Matrix Multiplication.
vector<vector<int>> strassenMultiply(vector<vector<int>>A, vector<vector<int>>B, int size)
{
 //If the size of the matrix is one.
 if(size == 1)
 {
 vector<vector<int>>C(size,vector<int>(size,0));
 C[0][0] = A[0][0]*B[0][0];
 return C;
 }
 vector<vector<int>>C(size,vector<int>(size,0));
 int k = size/2;
 //Dividig the matrix.
 vector<vector<int>>A11(k,vector<int>(k,0));
 vector<vector<int>>A12(k,vector<int>(k,0));
 vector<vector<int>>A21(k,vector<int>(k,0));
 vector<vector<int>>A22(k,vector<int>(k,0));
 vector<vector<int>>B11(k,vector<int>(k,0));
 vector<vector<int>>B12(k,vector<int>(k,0));
 vector<vector<int>>B21(k,vector<int>(k,0));
 vector<vector<int>>B22(k,vector<int>(k,0));
 //Storing those matrix in smaller matrix
 for(int i=0; i<k; i++)
 {
 for(int j=0; j<k; j++)
 {
 A11[i][j] = A[i][j];
 A12[i][j] = A[i][k+j];
 A21[i][j] = A[i+k][j];
 A22[i][j] = A[i+k][j+k];
 B11[i][j] = B[i][j];
 B12[i][j] = B[i][k+j];
 B21[i][j] = B[i+k][j];
 B22[i][j] = B[i+k][j+k];
 }
 }
vector<vector<int>>P = strassenMultiply(add(A11,A22,k),add(B11,B22, k), k);
 vector<vector<int>>Q = strassenMultiply(add(A21,A22,k),B11,k);
 vector<vector<int>>R = strassenMultiply(A11,subtract(B12,B22,k),k);
 vector<vector<int>>S = strassenMultiply(A22,subtract(B21,B11,k),k);
 vector<vector<int>>T = strassenMultiply(add(A11,A12,k),B22,k);
 vector<vector<int>>U = strassenMultiply(subtract(A21,A11,k),add(B11,B12,k), k);
 vector<vector<int>>V = strassenMultiply(subtract(A12,A22,k),add(B21,B22,k), k);
vector<vector<int>>C11 = add(P,add(subtract(S,T,k),V,k),k);
 vector<vector<int>>C12 = add(R,T,k);
 vector<vector<int>>C21 = add(Q,S,k);
 vector<vector<int>>C22 = add(P,add(subtract(R,Q,k),U,k),k);
for(int i=0; i<k; i++)
 {
 for(int j=0; j<k; j++)
 {
 C[i][j] = C11[i][j];
 C[i][j+k] = C12[i][j];
 C[i+k][j] = C21[i][j];
 C[i+k][j+k] = C22[i][j];
 }
 }
 return C;
}