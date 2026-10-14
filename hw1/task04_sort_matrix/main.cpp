#include <cstdlib>
#include <iostream>

long long Determinant(int* const* matrix) {
  return static_cast<long long>(matrix[0][0]) * matrix[1][1] -
         static_cast<long long>(matrix[0][1]) * matrix[1][0];
}

int CompareMatrices(const void* left, const void* right) {
  int** a = *static_cast<int** const*>(left);
  int** b = *static_cast<int** const*>(right);

  const long long determinant_a = Determinant(a);
  const long long determinant_b = Determinant(b);
  return (determinant_a > determinant_b) - (determinant_a < determinant_b);
}

int main() {
  int n = 0;
  if (!(std::cin >> n) || n <= 0) {
    std::cout << "error\n";
    return 1;
  }

  int*** matrices = new int**[n];
  for (int i = 0; i < n; ++i) {
    matrices[i] = new int*[2];
    for (int y = 0; y < 2; ++y) {
      matrices[i][y] = new int[2]{};
      for (int x = 0; x < 2; ++x) {
        std::cin >> matrices[i][y][x];
      }
    }
  }

  const bool valid_input = static_cast<bool>(std::cin);
  if (valid_input) {
    std::qsort(matrices, n, sizeof(matrices[0]), CompareMatrices);

    for (int i = 0; i < n; ++i) {
      for (int y = 0; y < 2; ++y) {
        std::cout << matrices[i][y][0] << ' ' << matrices[i][y][1] << '\n';
      }
      std::cout << "Determinant: " << Determinant(matrices[i]) << '\n';
    }
  } else {
    std::cout << "error\n";
  }

  for (int i = 0; i < n; ++i) {
    for (int y = 0; y < 2; ++y) {
      delete[] matrices[i][y];
    }
    delete[] matrices[i];
  }
  delete[] matrices;

  return valid_input ? 0 : 1;
}
