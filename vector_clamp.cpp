// vector_clamp.cpp
// Демонстрация работы с std::vector.
#include <vector>
#include <sstream>
#include <cassert>
#include <cmath> // nan, isnan в test_clamp

using namespace std;

/// "Загоняет" value в отрезок [min_bound, max_bound].
template <class Val>
inline Val clamp(Val value, Val min_bound, Val max_bound)
{
  return value < min_bound ? min_bound
       : max_bound < value ? max_bound
       : value;
}

/// Проверим clamp.
int test_clamp()
{
  if (clamp(1, 0, 10) != 1) return 1;
  if (clamp(-1, 2, 5) != 2) return 2;
  if (clamp(10, 2, 5) != 5) return 3;
  if (!isnan(clamp(nan(""), 0., 1.))) return 4;
  return 0;
}


/// Применяет clamp к вектору, создаёт новый вектор того же типа с результатами.
template <class Val>
vector<Val> clamped(const vector<Val> &values, Val min_bound, Val max_bound)
{
  vector<Val> clv(values.size());
  for (size_t i = 0, sz = values.size(); i < sz; ++i)
    clv[i] = clamp(values[i], min_bound, max_bound);
  return clv;
}


/// Читает вектор значений типа Val из потока ввода in.
/// Столько, сколько сможет.
template <class Val>
vector<Val> read_vector(istream &in)
{
  vector<Val> values;
  for (Val val; in >> val;)
    values.push_back(val);
  in.clear(ios::failbit);
  values.shrink_to_fit();
  return values;
}

/// Тест одновременно read_vector и clamped.
int test_clamped()
{
  stringstream test;
  test << "10 4 -5. 2 51 2222.022 4 1.2 25 7 8.5 -";
  auto values = read_vector<float>(test);
  if (clamped(values, 0.f, 100.f) != vector<float>
      { 10.f, 4.f, 0.f, 2.f, 51.f, 100.f, 4.f, 1.2f, 25.f, 7.f, 8.5f })
    return 1;
  if (clamped(values, 3.f, 8.f) != vector<float>
      { 8.f, 4.f, 3.f, 3.f, 8.f, 8.f, 4.f, 3.f, 8.f, 7.f, 8.f })
    return 2;
  return 0;
}


int main()
{
  assert(test_clamp() == 0);
  assert(test_clamped() == 0);
  return EXIT_SUCCESS;
}
