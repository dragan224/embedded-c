// MRU (Most Recently Used) cache (O(N)/O(1) algoritam - pretraga i update)
// moze se dovesti do (O(1)/O(1)) alogirtma koriscenjem hash-mape,
// ali nam je u cilju minimizovati memoriju (+ velicina kes memorije je dovoljno mala za O(N)).

// kompajlovanje: g++ cache.cpp -std=c++11 -o cache
// pokretanje ./cache -detailed_output=<true|false> -custom_input=<true|false> -cache_size=<int> \\
// -test_case_size=<int> -address_space=<int> -output_cache_state=<true|false>

// flagovi:
// detailed_output - da li se prikazuje detaljna analiza cache, ukljucujuci i stanje kesa posle svakog
// trazenja u kesu
// custom_input - ako je true trazi se od korisnika da unese kljuceve trazenja u kesu, ako je false
// generisu se test primeri
// cache_size = velicina kesa, tj. koliko kljuceva staje u kesu
// address_space = adresni opseg po defaultu je 256 (tj od 0 do 255)
// test_case_size = velicina svakog test primera.
// output_cache_state = default false, da li ispisujemo stanje kesa posle svakog ubacivanja
// REDOSLED FLAGOVA JE NEBITAN

// primer: ./cache -cache_size=64 -detailed_output=true

#include <cctype>
#include <iomanip>
#include <iostream>
#include <list>
#include <cmath>
#include <random>
#include <fstream>
#include <vector>

using namespace std;

// Flagovi
bool DETAILED_OUTPUT = false;
bool CUSTOM_INPUT = false;
bool OUTPUT_CACHE_STATE = false;
int CACHE_SIZE = 16;
int ADDRESS_SPACE = 256;
int TEST_CASE_SIZE = 10000;

bool BeginsWith(char * str, const string& cmp) {
  if (str == NULL || *str != '-') return false;
  str++;
  int i = 0;
  while (i < cmp.size()) {
    if (str == NULL) return false;
    if (tolower(*str) != cmp[i]) return false;
    i++; ++str;
  }
  return true;
}

void TryParseBool(char * str, const string& cmp, bool& output) {
  if (!BeginsWith(str, cmp)) return;
  while (str != NULL && *str != '=') ++str;
  if (str == NULL) return;
  ++str;
  if (str == NULL) {
    output = true;
  } else {
    tolower(*str) == 't' ? output = true : output = false;
  }
}

void TryParseInt(char * str, const string& cmp, int& output) {
  if (!BeginsWith(str, cmp)) return;
  while (str != NULL && *str != '=') ++str;
  if (str == NULL) return;
  ++str;
  int x = 0;
  while (str != NULL) {
    if (isdigit(*str)) {
      x = x * 10 + (*str) - '0';
    } else break;
    ++str;
  }
  output = x;
}

void InitFlags(int argc, char * argv[]) {
  for (int i = 0; i < argc; i++) {
    TryParseBool(argv[i], "detailed_output", DETAILED_OUTPUT);
    TryParseBool(argv[i], "custom_input", CUSTOM_INPUT);
    TryParseBool(argv[i], "custom_input", OUTPUT_CACHE_STATE);
    TryParseInt(argv[i], "cache_size", CACHE_SIZE);
    TryParseInt(argv[i], "address_space", ADDRESS_SPACE);
    TryParseInt(argv[i], "test_case_size", TEST_CASE_SIZE);
  }
}
// Flagovi

template<typename K>
int FETCH_FROM_MEMORY(K key) {
  // vraca vrednost sa kljucem K iz glavne memorije
  // posto je ovo simulacija, ta vrednost nam nije bitna
  return 42;
}

template <class K, class V>
class CacheEntry {
 public:
  CacheEntry(K key, V value) : key_(key), value_(value) {}
  K key() { return key_; }
  V value() { return value_; }

 private:
  K key_;
  V value_;
};

template <class V>
class Pair {
 public:
  explicit Pair(V data, bool is_found) : data_(data), is_found_(is_found) {}
  explicit Pair(bool is_found) : is_found_(is_found) {}
  V data() { return data_; }
  bool is_found() { return is_found_; }

 private:
  V data_;
  bool is_found_;
};


template <class K, class V>
class Cache {
 public:
  Cache(int size) : size_(size) { miss_ = hit_ = 0; }
  Pair<V> get(K key) {
    // trazi kljuc K u kesu i praca par gde
    // je prvi element para podatak koji se trazi
    // a drugi element true ili false (da li je dati kljuc pronadjen)
    if (OUTPUT_CACHE_STATE) {
      cout << "Trenutni kes: ";
      for (auto it = memory_.begin(); it != memory_.end(); it++) {
        cout << it->key() << " ";
      }
      cout << endl;
    }

    for (auto it = memory_.begin(); it != memory_.end(); it++) {
      if (it->key() == key) {
        auto temp = *it;
        memory_.erase(it);
        memory_.push_front(temp);
        hit_++;
        return Pair<V>(it->value(), true);
      }
    }
    // ubacivanje u kes
    miss_++;
    if (memory_.size() == size_) memory_.erase(memory_.begin());
    memory_.push_front(CacheEntry<K, V>(key, FETCH_FROM_MEMORY(key)));
    return Pair<V>(false);
  }
  void OutputStatistics() {
    cout << fixed << "Broj pogodka: " << miss_ << "/" << hit_ + miss_ << " (" << 100.0*miss_/(miss_+hit_) << "%)\n";
    cout << fixed << "Broj promasaja: " << hit_ << "/" << hit_ + miss_ << " (" << 100.0*hit_/(miss_+hit_) << "%)\n";
    cout << endl;
  }
 private:
  int size_;
  int miss_;
  int hit_;
  list<CacheEntry<K, V> > memory_;
};

void GenerateTestCases() {
  srand(time(NULL));
   
  // random test_case (generise random brojeve)
  // ovo se retko desava u praksi (jer u glavnom postoji neka lokalnost vezana za kes)
  // ocekivana verovatnoca pogodtka je CACHE_SIZE/ADDRESS_SPACE za dovoljno velike test primeri
  {
    cout << "Ukupan broj zahteva (Random Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      int x = rand() % ADDRESS_SPACE;
      xs.push_back(x);
      cache.get(x);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // LRU BUSTER
  // algoritam na kome LRU ima 0 hitova
  {
    cout << "Ukupan broj zahteva (LRU test Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      if (i > CACHE_SIZE) {
        xs.push_back(xs[i - CACHE_SIZE]);
      } else {
        xs.push_back(i);
      }
      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // MRU BUSTER (trenutni kes)
  // algoritam na kome MRU ima 0 hitova
  {
    cout << "Ukupan broj zahteva (MRU test Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      if (i > CACHE_SIZE) {
        xs.push_back(xs[i - 2]);
      } else {
        xs.push_back(i);
      }
      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // Ciklicni algoritam
  // Hocemo redom adrese 0, 1, 2, 3, ..., MAX_ADDRESS, 0, 1, 2, 3 itd.
  {
    cout << "Ukupan broj zahteva (Cyclic Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      xs.push_back(i % ADDRESS_SPACE);
      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // Real life 1
  // Pomeramo se po normalnoj raspodeli u zadatom broju
  {
    cout << "Ukupan broj zahteva (Real life - 1 Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    xs.push_back(rand() % ADDRESS_SPACE);
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::normal_distribution<double> distribution(ADDRESS_SPACE / 2.0, ADDRESS_SPACE / 2.0);

    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      xs.push_back(int(round(distribution(generator))));

      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // Real life 2 
  // isto kao real life 1 samo sa vecom frangmentacijom
  {
    cout << "Ukupan broj zahteva (Real life - 2 Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::normal_distribution<double> distribution(ADDRESS_SPACE / 2.0, ADDRESS_SPACE / 4.0);

    for (int i = 0; i < TEST_CASE_SIZE; i++) {

      xs.push_back(int(round(distribution(generator))));

      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // Log skala
  // logaritamska skala
  // ovo simulari cinjenicu da su adrese kojima najcesce pristupamo najvise verovatne
  // pretpostavili smo da su to najmanje adrese.. tako da sto je veca adresa to ima manju
  // verovatnocu da ce se zatraziti
  {
    cout << "Ukupan broj zahteva (Log scale - 1 Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    xs.push_back(rand() % ADDRESS_SPACE);
    
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::lognormal_distribution<double> distribution(0.0, log(ADDRESS_SPACE));

    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      xs.push_back(int(round(distribution(generator))));

      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }

  // Log skala - 2 
  // kao log skala - 1 samo sa vecom standardnom devijacijom
  {
    cout << "Ukupan broj zahteva (Log scale - 2 Algorithm): " << TEST_CASE_SIZE << "\n";
    vector<int> xs;
    Cache<int, int> cache(CACHE_SIZE);
    xs.push_back(rand() % ADDRESS_SPACE);
    
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::lognormal_distribution<double> distribution(0.0, ADDRESS_SPACE);

    for (int i = 0; i < TEST_CASE_SIZE; i++) {
      xs.push_back(int(round(distribution(generator))));

      cache.get(xs[i]);
    }
    if (DETAILED_OUTPUT) {
      cout << xs.size() << "\n";
      for (auto it = xs.begin(); it != xs.end(); it++) {
        cout << *it << " ";
      }
      cout << endl;
    }
    cache.OutputStatistics();
  }
}

int main(int argc, char * argv[]) {
  InitFlags(argc, argv); 
  cout << "Simulacija MRU cache algoritma! Dobrodosli!" << endl;
  if (DETAILED_OUTPUT) {
    cout << "Velicina kesa: " << CACHE_SIZE << "\n";
    cout << "Memorijske adrese su u opsegu od 0 do " << ADDRESS_SPACE-1 << "\n";
    cout << endl;
  }

  Cache<int, int> cache(CACHE_SIZE);
  if (CUSTOM_INPUT) {
    int n;
    int x;
    cin >> n;
    while (n--) {
      cin >> x;
      Pair<int> par = cache.get(x);
      if (par.is_found() && DETAILED_OUTPUT) {
        cout << "Pogodak: " << x << "\n";
      } else if (DETAILED_OUTPUT) {
        cout << "Promasaj: " << x << "\n";
      }
    }
    cache.OutputStatistics();
  } else {
    GenerateTestCases();
  }
  return 0; 
}
