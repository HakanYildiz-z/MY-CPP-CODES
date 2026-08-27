#include<iostream>      // ekrana herhangi bir çıktı basabilmek için
#include<string>        // normal harf dizileri için
#include<vector>        // genel dizi oluşumu için
#include<iomanip>       // çıktı ekranını manipüle edecek fonksiyonlar için
#include<memory>        // akıllı pointerler için
#include<utility>       // std::move kullanabilmek için
#include<limits>        // hata ayıklama ve akış temizliği için kullanılıyor

class Employee {   // ana sınıfımızdır.
protected:      // private verilerine alt sınıflar doğrudan erişemez ve onları yazdıramaz. ama protected verilerini yazdırablir.

    std::string name;       // string olarak çalışan isimlerini tanımlıycaz.
    double BaseSalary;      // double türü ile ana maaşları belirliycez.

public:

    Employee(std::string name , double BaseSalary) : name(name), BaseSalary(BaseSalary) {}   // kurucu fonksiyon

    virtual ~Employee() = default;   // yıkıcı fonksiyon  alt sınıfların da yok edildiğinden emin olmak için virtual olarak yazılmış.

    virtual double calculateSalary() const = 0 ;    // üst fonksiyonlar
    virtual void displayRole() const = 0;

    void printDetails() const {    // bunu alt sınıflara vermiyoruz çünkü onların verilerini de vector yardımıyla zaten üst sınıfta toplayıp for ile çiktı vereceğiz.

        //  std::left fonksiyonu çıktıyı sola yaslar. std::setw fonksiyonu çıktının kaç kareye sıkıştırılacağını söyler.
        std::cout << " isim : " << name << std::left << std::setw(15) << name << " | " ;
        displayRole();       // vector içinde sırada hangi veri varsa onun bilgilerini çıktı vericek.

        //std::fixed sabit demektir. matematiksel sayı gösterimini engeller. set precision  hassaiyet ayarla demektir ve virgülden sonraki basamak sayısını belirler.
        std::cout << " | maas :" << std::fixed << std::setprecision(2) << calculateSalary() << " TL \n" ;
    }
};

class Intern : public Employee {
// intern yani stajyerler için yazılmış olan sınıftır.

public:
    Intern( std::string name ,double BaseSalary) : Employee(std::move(name),BaseSalary){}      // alt sınıfın kurucusunun doğrudan üst sınıftan almasını sağlıyoruz

    double calculateSalary()const override {        // sanal fonksiyona gövde yazıyoruz burada.   main içinde vektor yardımıyla maaş değerini düzenliycez
        return BaseSalary;
    }
    void displayRole() const override {
        std::cout << std::left << std::setw(15) << "stajyer" ;  // çıktıyı sola yasla ve 15 kareye sıkıştır diyoruz.

    }
};

class RegularEmployee : public Employee {

public:
        RegularEmployee ( std::string name , double BaseSalary) : Employee( std:: move(name), BaseSalary) {}

        double calculateSalary() const override {
            return BaseSalary;
        }
        void displayRole()const override {
            std::cout << std::left << std::setw(15) << "calisan ";
        }
};

class Manager : public Employee {
private:                              // müdürlere bonus vereceğimiz için burada private bloğu oluşturdum ve kurucu fonksiyonuma bunu ekleyeceğim.
    double Bonus;

public:
    Manager(std::string name , double BaseSalary , double Bonus) : Employee(std::move(name) , BaseSalary) , Bonus(Bonus) {}

    double calculateSalary() const override {
        return BaseSalary + Bonus;
    }
    void displayRole() const override {
        std::cout << std::left << std::setw(15) << "mudur";
    }
};

int main () {
    std::vector<std::unique_ptr<Employee>> employees;

    int choice = 0;            // bu kodumuzda seçim hakkı tanıyarak hangi grup hakkında bilgi almak istiyorsa doğrudan o gruba bakmasını sağlıyoruz.

    while (true) {  // whle döngüsü oluşturuyoruz ve biz kapatmadıkça kendisi kapanmaz.

        std::cout << "      PERSONEL YONETİM SİSTEMİ   \n\n";
        std::cout << "1 : yonetici ekle \n ";
        std::cout << "2 : calisan ekle \n";
        std::cout << "3 : stajyer ekle \n ";
        std::cout << "4 : sistemi kapat \n ";
        std::cout << "Seciminizi giriniz : ";
        std::cin >> choice;

        if (std::cin.fail()) {    // int türünde veri girilmiş mi diye sorguluyoruz.
            std::cin.clear();     // arka planda hatalı veri girişi olduğu için cin failbit çalıştırır. veri girişine devam edebilmek için bunu temizliyoruz.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');  // ignore cin in içini temizler içerdeki blok ise temizliğin boyutunu ayarlar. tampon belleğin hepsini kapla demiş orada.
            std::cout << "giris hatali tekrar dene \n ";
            continue;
        }

        if (choice == 4) {
            break;
        }
        std::string name;   // main e ait olan string ve salary değerlerini kullanıcıdan cin ile almak için tanımlıyoruz.
        double salary = 0;
        std::cout << "isim : ";
        std::cin >> name;
        std::cout << "  \n taban maas ";
        std::cin >> salary;

        if ( choice == 1) {
            double bonus = 0;
            std::cout << "bonus :" ;
            std::cin >> bonus;
            employees.push_back(std::make_unique<Manager>(name, salary, bonus));
        }else if (choice == 2) {
            employees.push_back(std::make_unique<RegularEmployee>(name, salary));
        }else if (choice == 3) {
            employees.push_back(std::make_unique<Intern>(name, salary));
        }else {
            std::cout << "gecersiz secim ";
        }
    }

    std::cout << "\n  sirket maas raporu \n ";
    for (const auto& emp :employees) {   // hemyaptığımız gibi public kısmına erişip oradaki fonksiyonu aktif ediyoruz.
        emp->printDetails();
    }
return 0;
}
