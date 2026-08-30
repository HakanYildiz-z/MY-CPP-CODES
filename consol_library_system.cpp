#include<iostream>    // giriş çıkış için
#include<iomanip>     // giriş çıkış ekranının düzenini manipüle etmek için
#include<string>      // diziler için
#include<vector>      // vector dizileri için
#include<memory>      // akıllı pointerlar için
#include<algorithm>   // arama sıralama ve değiştirme/ filtreleme operatörlerini barındırır.
#include<utility>     // std::move kullanmamızı sağlıyor.
#include<limits>      // std::cin üzerindeki hataları kontrol etmek için yazdık bunu

class book {   // bu kütüphane kitap bilgilerini tutacak.
private:
    int id = 0;                 // kitap id si
    std::string title;          // kitap ismi
    std::string author;         // yazar ismi
    int publicationYear = 0;    // basım yılı
    bool isBorrowed ;           // ödünç durumu

public:

    // kurucu fonksiyonda veri akışını daha hızlı gerçekleştirebilmek için stringleri std::move içinde eşitliyoruz.
    book( int id , std::string title , std::string author , int publicationYear )      //bool isBorrowed  yazmadık içeriye çünkü kitap daha ilk tanımlanırken ödünç alınmış mı diye dış dünyaya sormak mantıksız zaten yeni tanımlanıyor.
        : id(id) , title(std::move(title)) , author(std::move(author)) , publicationYear(publicationYear) , isBorrowed(false) {}

// elimizdeki verileri dış dünya ya vermek için biz genelde fonksiyon yazarız burada ise bizden beş farklı veriyi dış dünyaya yazdırmamız bekleniyor beş farklı fonksiyon ile bunu yapalım.
    int get_id() const {return id;}       // pek bir olay yok sadece fonksiyon return olarak mevcut veriyi döderiyor.
    const std::string& get_title() const {return title;}          // burada kopyalama maliyetine girmemek için referansla& veriyi çağırıyoruz bu referans işlemini güvenle yapabilmek iiçin başına tekrar const yazıyoruz .
    const std::string& get_author() const {return author;}      // baştaki const referansın değişmesini engellerken sondaki const nesnenin değişmesini engelliyor.
    int get_publicationYear() const {return publicationYear;}
    bool get_isBorrowed() const {return isBorrowed;}

    void set_isBorrowed(bool status) {isBorrowed = status;}    // kitabın ödünçlük durumunu güncellemek için yazdık

    void displayDetails() const {
        std::cout << "ID :" << std::left << std::setw(5) << id
                  << "ISIM :"<< std::left << std::setw(25) << title
                  << "YAZAR :"<< std::left << std::setw(25) << author
                  << "BASIM YILI :"<< std::left << std::setw(5)<<publicationYear
                  << "DURUM :" << (isBorrowed ? "kitap mevcut degil ": "kitap mevcut") <<"\n";
    }
};
class library {   // bu sınıf kütüphanenin verilerini tutar.
private:
    std::vector<book> book_collection;  // üst kütüphanenin türünde bir vector dizisi oluşturduk. burası Book kütüphanesinin kışlası gibi olucak veriler burada depolanacak.
    int next_id = 1;

public:
    void add_book(std::string title , std::string author , int publicationYear) { // book sınıfının privatesine veri veriyoruz. veri verme işlemi yaptığımıza bakma veri alamayız o ibneden.
        book_collection.emplace_back(next_id ++ , std::move(title) , std::move(author),publicationYear);   // emplace_back fonksiyonu ile verileri doğrudan hafızaya kaydederiz. vektor dizisi olarak bütün şeklinde hafızaya kaydedilir bu veriler
    }

    // auto otomatik olarak türün ne olduğunu derleyiciye buldurur. it ise bulunan türün değişkenidir .
    // std::remove_if() fonksiyonu verinin kullanımını engelleyerek onu hafızanın sonuna sürgün eder.
    // cpp deki vectorler bir konteynerdir içinde farklı nesneler barındırırlar .begin() ve .end() fonksiyonları bu onteynerin başından sonuna kadar erişmemizi sağlar.
    // []() {}  bu bir lambda fonksiyonunun gövdesdir lambda fonksiyonları genel olarak nomal fonksiyonlarla neredeyse aynı işlevi görür. ek maliyet olmadan anlık olarak yazılabilir.
    // [] parantez içine dışarıdan gelen verileri koyarız burada id verisi dışarıdan geliyor. b ise doğrudan hafızadan alınıyor.
    // .erase() fonksiyonu başlangıç ve bitiş noktasını aldığı veriyi hafızadan paket ediyor.
    bool remove_book(int id) {
        auto it = std::remove_if(book_collection.begin(), book_collection.end() , [id] (const book& b){return b.get_id() == id;});

        if (it != book_collection.end()) {                                    // sona ulaşmadıysa yani it bitmediyse diye sorguluyor. it bitmediyse şunu yap.
            book_collection.erase(it , book_collection.end());  // it içinde hem başlangıç hemde bitişle birlikte olan kapsama alanı var.
            return true;                                                                  // biz yine de erase istiyor diye bitişi tekrardan yazıyoruz. neden ?? çünkü erase öyle istiyor.
        }
        return false;
    }

    void list_books() const {  // burada değişiklik yapmayacağımız için const yazdık

        if (book_collection.empty()) {   // empty fonksiyonu bu konteyner boş mu diye soruyor.
            std::cout <<"arsivde böyle bir eser yok  \n";
        }

        std::cout << "\n---KUTUPHANE ARSIVI---\n " ;
        for ( const auto& book : book_collection) {    // buradaki referansı kopyalama maliyetini sıfırlamak için yazıyoruz.
            book.displayDetails();             // kitap sınıfının yazdırma fonksiyonunu kullanıyoruz. verileri dışarıya çıktı olarak sunacağız.
        }
    }

    void search_books(const std::string& query) const {   // string olan isim ve yazar üzerinden arama yapabileceğimiz bir fonksiyon yazıyoruz ve bunun içine o sorgu verisini atıyoruz.
        bool found = false;    // bulundu mu değerimiz ilk anda false
        std::cout << "\n---ARAMA SONUCLARI---\n";
        for (const auto& book : book_collection ) {

            //.find() fonksiyonu bir nesnenin içinde arama yaparak parantezinin içindeki veri orada mevcut mu diye sorgular.
            // std::string türü içinde yer alan npos no pozitiondan gelir ve strink olan böyle bir veri yok der. biz kodda string olan böyle bir veri yok komutu yanlışsa yani böyle bir veriyi bulduysak devam edlim diyoruz
            if (book.get_title().find(query) != std::string::npos || book.get_author().find(query) != std::string::npos) {
                book.displayDetails();
                found = true;
            }
        }
        if (!found) {
            std::cout <<" eslesen bir kitap yoktur ";
        }
    }
    // ödünç alma ve iade etme durumu güncellemesini yapıyoruz ve false veya true değerini dışa aktarıyoruz

    bool toggle_borrow_status(int id , bool status) {
        for (auto& book : book_collection) {
            if (book.get_id() == id) {                   // bu id değerine sahip miyiz diye sorguluyoruz.

                if (book.get_isBorrowed() == status) {   // eğer bu kitabın ödünçlük durumu zaten status halde yani itenen durumda ise kodu false ile kapatıyoruz.
                    return false ;
                }
                book.set_isBorrowed(status);             // get fonksiyonumuz ile koontrol yaptıktan sonra get ile yeni değeri atıyoruz
                return true;                             // bu kısımda else kullanmıyoruz çünkü zaten içerideki sağlanırsa return anında ana fonksiyonu kapatır
            }
        return false;
        }
    }
};

// cin düznini sağlamak için bağımsız bir fonksiyon yazıyoruz.
void clear_inputStream() {        // cin arka planını tamamen temizliyoruz ki kod içerisinde hatalı çıktılar çıkmasın.
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max() , '\n');     // cin karakter verisi aldığı için \n "" yerine '' ile kapatıldı.
}

//MAİN FONKSİYONU

int main() {
library mylibrary;            // library türünde library değişkenini oluşturuyoruz.
int choice = 0;
while (true) {
    // bu döngüde yukarıda yazdığımız fonksiyonların uygulamalarını yapıyoruz.
    std::cout << "\n--- KUTUPHANE YONETIM SISTEMI ---\n";
    std::cout << "1. Kitap Ekle \n";
    std::cout << "2. Kitaplari Listele \n";
    std::cout << "3. Kitap Ara \n";
    std::cout << "4. Kitap Sil \n";
    std::cout << "5. Odunc Durumu Degistir \n";
    std::cout << "6. Cikis \n";
    std::cout << "Seciminiz : ";

    if (!(std::cin >> choice)) {         // if gözetiminde choice verisini cin ile alıyoruz. alamazsak ! kontrolu ile if içine düşeriz.
        if (std::cin.eof()) break;       // eğer kullanıcı manuel olarak cin akışını bitirdiyse otomatik olarak kodu kapat diyoruz. cin.eof() akış devam ediyor mu sorusunu soruyor eğer not ise yani ! kapatıyor.
        clear_inputStream();
        std::cout << "\n hatali giris yapildi lütfen tekrar deneyin \n";
        continue;
    }
    if (choice == 6) {
        std::cout << "Sistemden cikiliyor... Disiplini koru! (Exiting system... Maintain discipline!)\n";
        break;
    }
    if (choice == 1) {   // kitap ekle seçneğini inşa ediyoruz.
        std::string title , author ;
        int publicationYear;

        clear_inputStream();   // cin girişi almadan önce veri etmizliği yapıyoruz. güvenlik önlemi

        std::cout << " Kitap adi :";
        std::getline(std::cin, title );      // getline fonksiyonu bizim cin yazımı içindeki yer alacak olan boşlukların kullanımına izin verir. yani yazdığımız şeyi yazabilmemizi sağlar.
        // normalde cin ile veri girişi alırken string verilerinde sadece tek kelime yazabiliriz 2. yazmaya kalkarsak cin bunu okuyamaz ve o 2. string buffere düşer ve bir sonraki cin laımında çöp veri olarak önümüze düşer.
        std::cout << "yazar adi :";
        std::getline(std::cin , author);

        while (true) {
            std::cout << "basim yili :";
            if (std::cin >> publicationYear && publicationYear >= 0 && publicationYear <= 2026 ) {  // eğer cin verisini doğru sınırlar iiçinde alırsak if bloğu çalışır ve dögü sona erer.
                break;
            }
            clear_inputStream();     // cin içindeki çöpverileri temziliyoruz.
            std::cout << " hatali giris yaptiniz lutfen tekrar deneyiniz. ";
        }
        mylibrary.add_book(title , author , publicationYear);
        std::cout << "kitap basari ile kutuphaneye yüklendi. ";

    }else if (choice == 2) {     // listeleme fonksiyonumuzu çalıştırmamız yeterli

        mylibrary.list_books();

    }else if (choice == 3) {
        std::string query;
        clear_inputStream();
        std::cout << " \n lutfen  arama verilerinizi girin :";
        std::getline(std::cin , query);
        mylibrary.search_books(query);

    }else if (choice == 4) {
        int id = 0 ;
        std::cout << " \n lutfen silmek istediginiz kitabin id sini giriniz :";
        clear_inputStream();
        std::cin >> id ;
        mylibrary.remove_book(id);
        std::cout << "silmek istedeginiz kitabin silme islemi tamamlandi";

    }else if (choice == 5) {
        int id = 0 ;
        int status = 0 ;
        clear_inputStream();
        std::cout << "kitap id sini girin lutfen ";
        std::cin >> id;
        std::cout <<"kitap iadesi icin 1 e kitap odunc almak icin 2 ye basin seciminiz :";
        std::cin >> status;
        if (status == 1 || status == 2) {
            bool new_status = (status == 1);  // status == 1; doğruysa true yanlışsa false sonucu çıkar ve bu new_status değerine atanır.
            if (mylibrary.toggle_borrow_status(id , new_status)) {    // eğer güncellenmek istenen değer ile mevcut değer aynıysa hata vericek şekilde fonksiyonu tasarladık.
                std::cout << " oduncluk durumu guncellendi";
            }else {
                std::cout <<"islem gerceklestirilemedi id degerinde hata var veya durum zaten istendigi gibi.";
            }

        }else {
            std::cout <<" gecersiz secim";
        }
        clear_inputStream();

        }else {
            std::cout << "gecersiz secim ";
        }
    }


}
