#include <iomanip>
#include <iostream>

struct MinMax {
  int min;
  int max;
};

double Average(const int* temperatures, int n) {
  long long sum = 0;

  for (int i = 0; i < n; ++i) {
    sum += temperatures[i];
  }

  return static_cast<double>(sum) / n;
}

MinMax FindMinMax(const int* temperatures, int n) {
  MinMax result{temperatures[0], temperatures[0]};
  for (int i = 1; i < n; i ++){
    if (temperatures[i] < result.min){
      result.min = temperatures[i];
    }
    else if (temperatures[i] > result.max){
      result.max = temperatures[i];
    }
  }
  return result;
}

int CountFreezingDays(const int* temperatures, int n) {
  int count = 0;
  for (int i = 0; i < n; i ++){
    if (temperatures[i] < 0){
      count += 1;
    }
  }
  return count;
}

int main() {
  int n;

  std::cin >> n;

  int* temperatures = new int[n];

  for (int i = 0; i < n; ++i) {
    if (!(std::cin >> temperatures[i])) {
      delete[] temperatures;
      return 1;
    }
  }

  const double cMean = Average(temperatures, n);
  const MinMax cExtremes = FindMinMax(temperatures, n);
  const int cFreezing_days = CountFreezingDays(temperatures, n);

  std::cout << std::fixed << std::setprecision(1) << cMean << '\n';
  std::cout << cExtremes.min << ' ' << cExtremes.max << '\n';
  std::cout << cFreezing_days << '\n';

  delete[] temperatures;
  return 0;
}
