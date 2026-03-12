#include "iostream"
#include "chrono"
#include "vector"
#include "random"

void step(float* r, const float* d, int n);


int main() {
    int n = 6000;
    // 메모리 할당
    std::vector<float> d(n * n);
    std::vector<float> r(n * n);

    // 난수 생성 (0.0 ~ 1.0)
    std::mt19937 gen(1337);
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    for (int i = 0; i < n * n; ++i) d[i] = dis(gen);

    // 시간 측정 시작
    auto start = std::chrono::high_resolution_clock::now();

    step(r.data(), d.data(), n);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // 결과 출력 (컴파일러 최적화 방지용)
    std::cout << "Time: " << diff.count() << " s" << std::endl;
    std::cout << "Result check: " << r[0] << std::endl;

    return 0;
}

void step(float* r, const float* d, int n){
  #pragma omp parallel for
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      float min_val = d[i*n + j];
      asm("# foo");
      for(int k = 0; k < n; ++k) {
        min_val = std::min(min_val, d[i*n + k] + d[k*n + j]);
      }
      r[i*n + j] = min_val;
    }
  }
}
