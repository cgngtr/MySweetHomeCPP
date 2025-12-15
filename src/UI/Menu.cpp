#include "Menu.h"
#include "ConsoleUtils.h"
#include "SmartHome.h"
#include "SecurityManager.h"
#include "Device.h"
#include "Light.h"
#include "Camera.h"
#include "Logger.h"
#include <iostream>
#include <sstream>

namespace MySweetHome {

Menu::Menu(SmartHome* smartHome)
    : m_smartHome(smartHome)
    , m_running(false)
{
}

Menu::~Menu()
{
}

void Menu::run() {
    m_running = true;
    Logger::getInstance().info("Menu baslatildi.");

    while (m_running) {
        showMainMenu();

        int choice = ConsoleUtils::getIntInput("Seciminiz: ");

        switch (choice) {
            case 1:
                getHomeStatus();
                break;
            case 2:
                addDevice();
                break;
            case 3:
                removeDevice();
                break;
            case 4:
                powerOnDevice();
                break;
            case 5:
                powerOffDevice();
                break;
            case 6:
                changeMode();
                break;
            case 7:
                changeState();
                break;
            case 8:
                showManual();
                break;
            case 9:
                showAbout();
                break;
            case 10:
                shutdown();
                break;
            case 11:
                simulateSecurityEvent();
                break;
            case 12:
                simulateDeviceFailure();
                break;
            default:
                ConsoleUtils::printError("Gecersiz secim! Lutfen 1-10 arasi bir sayi girin.");
                ConsoleUtils::pause();
                break;
        }
    }
}

void Menu::showMainMenu() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("EVIM EVIM GUZEL EVIM");

    std::cout << std::endl;
    std::cout << "  Mod: " << m_smartHome->getCurrentModeString();
    std::cout << " | Durum: " << m_smartHome->getCurrentStateString();
    std::cout << " | Cihazlar: " << m_smartHome->getActiveDeviceCount();
    std::cout << "/" << m_smartHome->getDeviceCount() << " aktif" << std::endl;
    std::cout << std::endl;

    printMenuOption(1, "Ev Durumunu Goruntule");
    printMenuOption(2, "Cihaz Ekle (I/D/K/T)");
    printMenuOption(3, "Cihaz Kaldir (I/D/K/T)");
    printMenuOption(4, "Cihazi Ac (I/D/K/T/S)");
    printMenuOption(5, "Cihazi Kapat (I/D/K/T/S)");
    printMenuOption(6, "Mod Degistir (N/A/P/S)");
    printMenuOption(7, "Durum Degistir (N/Y/D/U/O)");
    printMenuOption(8, "Kullanim Kilavuzu");
    printMenuOption(9, "Hakkinda");
    printMenuOption(10, "Sistemi Kapat");
    std::cout << std::endl;
}

void Menu::printMenuOption(int number, const std::string& text) {
    std::cout << "  [";
    if (number < 10) std::cout << " ";
    std::cout << number << "] " << text << std::endl;
}

void Menu::getHomeStatus() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("EV DURUMU");

    std::cout << std::endl;
    std::cout << "  Sistem Modu    : " << m_smartHome->getCurrentModeString() << std::endl;
    std::cout << "  Sistem Durumu  : " << m_smartHome->getCurrentStateString() << std::endl;
    std::cout << "  Toplam Cihaz   : " << m_smartHome->getDeviceCount() << std::endl;
    std::cout << "  Aktif Cihaz    : " << m_smartHome->getActiveDeviceCount() << std::endl;
    std::cout << std::endl;

    ConsoleUtils::printSeparator('-', 60);
    std::cout << "  KAYITLI CIHAZLAR:" << std::endl;
    ConsoleUtils::printSeparator('-', 60);

    listDevices();

    std::cout << std::endl;
    ConsoleUtils::pause();
}

void Menu::addDevice() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("CIHAZ EKLE");

    std::cout << std::endl;
    std::cout << "  Cihaz Tipini Secin:" << std::endl;
    std::cout << "  [I] Isik" << std::endl;
    std::cout << "  [D] Duman ve Gaz Dedektorleri" << std::endl;
    std::cout << "  [K] Kamera" << std::endl;
    std::cout << "  [T] TV (Samsung/LG)" << std::endl;
    std::cout << "  [S] Ses Sistemi" << std::endl;
    std::cout << "  [0] Iptal" << std::endl;
    std::cout << std::endl;

    char type = getCharInput("Seciminiz: ");

    if (type == '0') return;
    std::cout << std::endl;
    int quantity = ConsoleUtils::getIntInput("Adet girin (1-10): ");
    if (quantity < 1 || quantity > 10) {
        ConsoleUtils::printError("Gecersiz adet! 1-10 arasi olmali.");
        ConsoleUtils::pause();
        return;
    }

    int devicesAdded = 0;
    Device* firstDevice = 0;
    if (type == 'D' || type == 'd') {
        for (int i = 0; i < quantity; ++i) {
            std::ostringstream oss;
            oss << "Dedektor " << (i + 1) << " Adi: ";
            std::string name = ConsoleUtils::getInput(oss.str());
            std::string location = ConsoleUtils::getInput("Konum: ");
            m_smartHome->addDetectorPair(name, location);
            devicesAdded += 2;
        }
        std::ostringstream msg;
        msg << devicesAdded << " Dedektor basariyla eklendi (Duman + Gaz cifti).";
        ConsoleUtils::printSuccess(msg.str());
        ConsoleUtils::pause();
        return;
    }
    std::string name;
    std::string location;

    switch (type) {
        case 'I':
        case 'i': {
            name = ConsoleUtils::getInput("Isik Adi: ");
            location = ConsoleUtils::getInput("Konum: ");

            std::cout << std::endl;
            std::cout << "  Isik Yapilandirmasi:" << std::endl;
            int red = ConsoleUtils::getIntInput("  Renk - Kirmizi (0-255): ");
            int green = ConsoleUtils::getIntInput("  Renk - Yesil (0-255): ");
            int blue = ConsoleUtils::getIntInput("  Renk - Mavi (0-255): ");
            int brightness = ConsoleUtils::getIntInput("  Parlaklik (0-100): ");
            if (red < 0) red = 0; if (red > 255) red = 255;
            if (green < 0) green = 0; if (green > 255) green = 255;
            if (blue < 0) blue = 0; if (blue > 255) blue = 255;
            if (brightness < 0) brightness = 0; if (brightness > 100) brightness = 100;

            firstDevice = m_smartHome->addLight(name, location);
            if (firstDevice) {
                Light* light = static_cast<Light*>(firstDevice);
                light->setColor(static_cast<uint8_t>(red),
                               static_cast<uint8_t>(green),
                               static_cast<uint8_t>(blue));
                light->setBrightness(static_cast<uint8_t>(brightness));
                devicesAdded++;
            }
            break;
        }

        case 'K':
        case 'k': {
            name = ConsoleUtils::getInput("Kamera Adi: ");
            location = ConsoleUtils::getInput("Konum: ");

            std::cout << std::endl;
            std::cout << "  Kamera Yapilandirmasi:" << std::endl;
            bool motionDetection = ConsoleUtils::getYesNoInput("  Hareket Algilama Aktif mi?");
            int fps = ConsoleUtils::getIntInput("  FPS (1-60): ");
            bool nightVision = ConsoleUtils::getYesNoInput("  Gece Gorusu Aktif mi?");
            if (fps < 1) fps = 1; if (fps > 60) fps = 60;

            firstDevice = m_smartHome->addCamera(name, location);
            if (firstDevice) {
                Camera* camera = static_cast<Camera*>(firstDevice);
                camera->enableMotionDetection(motionDetection);
                camera->setFPS(fps);
                camera->setNightVision(nightVision);
                devicesAdded++;
            }
            break;
        }

        case 'T':
        case 't': {
            location = ConsoleUtils::getInput("Konum: ");
            std::cout << std::endl;
            std::cout << "  TV Markasi:" << std::endl;
            std::cout << "  [S] Samsung" << std::endl;
            std::cout << "  [L] LG" << std::endl;
            char brand = getCharInput("  Seciminiz: ");
            if (brand == 'S' || brand == 's') {
                firstDevice = m_smartHome->addSamsungTV(location);
            } else if (brand == 'L' || brand == 'l') {
                firstDevice = m_smartHome->addLGTV(location);
            } else {
                ConsoleUtils::printError("Gecersiz marka!");
                ConsoleUtils::pause();
                return;
            }
            if (firstDevice) devicesAdded++;
            break;
        }

        case 'S':
        case 's':
            name = ConsoleUtils::getInput("Ses Sistemi Adi: ");
            location = ConsoleUtils::getInput("Konum: ");
            firstDevice = m_smartHome->addSoundSystem(name, location);
            if (firstDevice) devicesAdded++;
            break;

        default:
            ConsoleUtils::printError("Gecersiz cihaz tipi!");
            ConsoleUtils::pause();
            return;
    }

    if (!firstDevice) {
        ConsoleUtils::printError("Ilk cihaz eklenemedi!");
        ConsoleUtils::pause();
        return;
    }

    ConsoleUtils::printSuccess("Cihaz 1 basariyla olusturuldu: " + firstDevice->getName());
    if (quantity > 1) {
        std::cout << std::endl;
        std::ostringstream prompt;
        prompt << "Yapilandirma kalan " << (quantity - 1) << " cihaza kopyalansin mi?";
        bool copyConfig = ConsoleUtils::getYesNoInput(prompt.str());

        if (copyConfig) {
            for (int i = 1; i < quantity; ++i) {
                Device* clone = firstDevice->clone();
                if (clone) {
                    if (m_smartHome->addClonedDevice(clone)) {
                        devicesAdded++;
                        std::ostringstream msg;
                        msg << "Cihaz " << (i + 1) << " basariyla klonlandi.";
                        ConsoleUtils::printSuccess(msg.str());
                    } else {
                        delete clone;
                        ConsoleUtils::printError("Cihaz klonlanamadi!");
                    }
                }
            }
        } else {
            for (int i = 1; i < quantity; ++i) {
                std::cout << std::endl;
                std::ostringstream header;
                header << "--- Cihaz " << (i + 1) << " / " << quantity << " ---";
                std::cout << header.str() << std::endl;

                Device* device = 0;

                switch (type) {
                    case 'I':
                    case 'i': {
                        name = ConsoleUtils::getInput("Isik Adi: ");
                        location = ConsoleUtils::getInput("Konum: ");
                        int red = ConsoleUtils::getIntInput("  Renk - Kirmizi (0-255): ");
                        int green = ConsoleUtils::getIntInput("  Renk - Yesil (0-255): ");
                        int blue = ConsoleUtils::getIntInput("  Renk - Mavi (0-255): ");
                        int brightness = ConsoleUtils::getIntInput("  Parlaklik (0-100): ");

                        if (red < 0) red = 0; if (red > 255) red = 255;
                        if (green < 0) green = 0; if (green > 255) green = 255;
                        if (blue < 0) blue = 0; if (blue > 255) blue = 255;
                        if (brightness < 0) brightness = 0; if (brightness > 100) brightness = 100;

                        device = m_smartHome->addLight(name, location);
                        if (device) {
                            Light* light = static_cast<Light*>(device);
                            light->setColor(static_cast<uint8_t>(red),
                                           static_cast<uint8_t>(green),
                                           static_cast<uint8_t>(blue));
                            light->setBrightness(static_cast<uint8_t>(brightness));
                        }
                        break;
                    }

                    case 'K':
                    case 'k': {
                        name = ConsoleUtils::getInput("Kamera Adi: ");
                        location = ConsoleUtils::getInput("Konum: ");
                        bool motionDet = ConsoleUtils::getYesNoInput("  Hareket Algilama Aktif mi?");
                        int fps = ConsoleUtils::getIntInput("  FPS (1-60): ");
                        bool nightVis = ConsoleUtils::getYesNoInput("  Gece Gorusu Aktif mi?");

                        if (fps < 1) fps = 1; if (fps > 60) fps = 60;

                        device = m_smartHome->addCamera(name, location);
                        if (device) {
                            Camera* camera = static_cast<Camera*>(device);
                            camera->enableMotionDetection(motionDet);
                            camera->setFPS(fps);
                            camera->setNightVision(nightVis);
                        }
                        break;
                    }

                    case 'T':
                    case 't': {
                        location = ConsoleUtils::getInput("Konum: ");
                        std::cout << "  TV Markasi [S]amsung/[L]G: ";
                        char brand = getCharInput("");
                        if (brand == 'S' || brand == 's') {
                            device = m_smartHome->addSamsungTV(location);
                        } else {
                            device = m_smartHome->addLGTV(location);
                        }
                        break;
                    }

                    case 'S':
                    case 's':
                        name = ConsoleUtils::getInput("Ses Sistemi Adi: ");
                        location = ConsoleUtils::getInput("Konum: ");
                        device = m_smartHome->addSoundSystem(name, location);
                        break;
                }

                if (device) {
                    devicesAdded++;
                    ConsoleUtils::printSuccess("Cihaz eklendi: " + device->getName());
                }
            }
        }
    }

    std::cout << std::endl;
    std::ostringstream finalMsg;
    finalMsg << devicesAdded << " cihaz basariyla eklendi.";
    ConsoleUtils::printSuccess(finalMsg.str());
    ConsoleUtils::pause();
}

void Menu::removeDevice() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("CIHAZ KALDIR");

    std::cout << std::endl;
    listDevices();

    if (m_smartHome->getDeviceCount() == 0) {
        ConsoleUtils::pause();
        return;
    }

    std::cout << std::endl;
    int id = ConsoleUtils::getIntInput("Kaldirilacak Cihaz ID (0=Iptal): ");

    if (id == 0) return;

    Device* device = m_smartHome->getDevice(static_cast<uint32_t>(id));
    if (device && device->isCritical()) {
        ConsoleUtils::printWarning("Kritik cihazlar kaldirilamaz: " + device->getName());
        ConsoleUtils::pause();
        return;
    }

    if (m_smartHome->removeDevice(static_cast<uint32_t>(id))) {
        ConsoleUtils::printSuccess("Cihaz basariyla kaldirildi.");
    } else {
        ConsoleUtils::printError("Cihaz bulunamadi veya kaldirilamadi!");
    }

    ConsoleUtils::pause();
}

void Menu::powerOnDevice() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("CIHAZI AC");

    std::cout << std::endl;
    listDevices();

    if (m_smartHome->getDeviceCount() == 0) {
        ConsoleUtils::pause();
        return;
    }

    std::cout << std::endl;
    int id = ConsoleUtils::getIntInput("Acilacak Cihaz ID (0=Iptal): ");

    if (id == 0) return;

    if (m_smartHome->powerOnDevice(static_cast<uint32_t>(id))) {
        ConsoleUtils::printSuccess("Cihaz acildi.");
    } else {
        ConsoleUtils::printError("Cihaz bulunamadi!");
    }

    ConsoleUtils::pause();
}

void Menu::powerOffDevice() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("CIHAZI KAPAT");

    std::cout << std::endl;
    listDevices();

    if (m_smartHome->getDeviceCount() == 0) {
        ConsoleUtils::pause();
        return;
    }

    std::cout << std::endl;
    int id = ConsoleUtils::getIntInput("Kapatilacak Cihaz ID (0=Iptal): ");

    if (id == 0) return;

    if (m_smartHome->powerOffDevice(static_cast<uint32_t>(id))) {
        ConsoleUtils::printSuccess("Cihaz kapatildi.");
    } else {
        Device* device = m_smartHome->getDevice(static_cast<uint32_t>(id));
        if (device && device->isCritical()) {
            ConsoleUtils::printWarning("Kritik cihazlar kapatilamaz!");
        } else {
            ConsoleUtils::printError("Cihaz bulunamadi!");
        }
    }

    ConsoleUtils::pause();
}

void Menu::changeMode() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("MOD DEGISTIR");

    std::cout << std::endl;
    std::cout << "  Mevcut Mod: " << m_smartHome->getCurrentModeString() << std::endl;
    std::cout << std::endl;

    std::cout << "  Mod Secenekleri:" << std::endl;
    std::cout << "  [N] Normal  - Isik ACIK, TV KAPALI, Muzik KAPALI" << std::endl;
    std::cout << "  [A] Aksam   - Isik KAPALI, TV KAPALI, Muzik KAPALI" << std::endl;
    std::cout << "  [P] Parti   - Isik ACIK, TV KAPALI, Muzik ACIK" << std::endl;
    std::cout << "  [S] Sinema  - Isik KAPALI, TV ACIK, Muzik KAPALI" << std::endl;
    std::cout << "  [0] Iptal" << std::endl;
    std::cout << std::endl;

    char choice = getCharInput("Seciminiz: ");

    switch (choice) {
        case 'N':
        case 'n':
            m_smartHome->setMode(MODE_NORMAL);
            ConsoleUtils::printSuccess("Mod Normal olarak degistirildi.");
            break;
        case 'A':
        case 'a':
            m_smartHome->setMode(MODE_EVENING);
            ConsoleUtils::printSuccess("Mod Aksam olarak degistirildi.");
            break;
        case 'P':
        case 'p':
            m_smartHome->setMode(MODE_PARTY);
            ConsoleUtils::printSuccess("Mod Parti olarak degistirildi.");
            break;
        case 'S':
        case 's':
            m_smartHome->setMode(MODE_CINEMA);
            ConsoleUtils::printSuccess("Mod Sinema olarak degistirildi.");
            break;
        case '0':
            return;
        default:
            ConsoleUtils::printError("Gecersiz mod!");
    }

    ConsoleUtils::pause();
}

void Menu::changeState() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("DURUM DEGISTIR");

    std::cout << std::endl;
    std::cout << "  Mevcut Durum: " << m_smartHome->getCurrentStateString() << std::endl;
    std::cout << std::endl;

    std::cout << "  Durum Secenekleri:" << std::endl;
    std::cout << "  [N] Normal" << std::endl;
    std::cout << "  [Y] Yuksek Performans" << std::endl;
    std::cout << "  [D] Dusuk Guc" << std::endl;
    std::cout << "  [U] Uyku" << std::endl;
    std::cout << "  [O] Onceki Durum" << std::endl;
    std::cout << "  [0] Iptal" << std::endl;
    std::cout << std::endl;

    char choice = getCharInput("Seciminiz: ");

    switch (choice) {
        case 'N':
        case 'n':
            m_smartHome->setState(STATE_NORMAL);
            ConsoleUtils::printSuccess("Durum Normal olarak degistirildi.");
            break;
        case 'Y':
        case 'y':
            m_smartHome->setState(STATE_HIGH_PERFORMANCE);
            ConsoleUtils::printSuccess("Durum Yuksek Performans olarak degistirildi.");
            break;
        case 'D':
        case 'd':
            m_smartHome->setState(STATE_LOW_POWER);
            ConsoleUtils::printSuccess("Durum Dusuk Guc olarak degistirildi.");
            break;
        case 'U':
        case 'u':
            m_smartHome->setState(STATE_SLEEP);
            ConsoleUtils::printSuccess("Durum Uyku olarak degistirildi.");
            break;
        case 'O':
        case 'o':
            if (m_smartHome->goToPreviousState()) {
                ConsoleUtils::printSuccess("Durum degistirildi: " + m_smartHome->getCurrentStateString());
            } else {
                ConsoleUtils::printWarning("Onceki durum yok!");
            }
            break;
        case '0':
            return;
        default:
            ConsoleUtils::printError("Gecersiz durum!");
    }

    ConsoleUtils::pause();
}

void Menu::showManual() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("KULLANIM KILAVUZU");

    std::cout << std::endl;
    std::cout << "  EVIM EVIM GUZEL EVIM - Akilli Ev Otomasyon Sistemi" << std::endl;
    std::cout << std::endl;
    std::cout << "  MENU SECENEKLERI:" << std::endl;
    std::cout << "  1. Ev Durumu        - Tum cihazlarin durumunu goster" << std::endl;
    std::cout << "  2. Cihaz Ekle       - Yeni cihaz ekle (Isik, Dedektor, Kamera, TV)" << std::endl;
    std::cout << "  3. Cihaz Kaldir     - Mevcut cihazi kaldir" << std::endl;
    std::cout << "  4. Cihazi Ac        - Cihazi ac (kritik cihazlar her zaman acik)" << std::endl;
    std::cout << "  5. Cihazi Kapat     - Cihazi kapat (kritik cihazlar kapatilamaz)" << std::endl;
    std::cout << "  6. Mod Degistir     - Sistem modunu degistir" << std::endl;
    std::cout << "  7. Durum Degistir   - Sistem durumunu degistir" << std::endl;
    std::cout << std::endl;
    std::cout << "  MODLAR:" << std::endl;
    std::cout << "  Normal - Isiklar acik, diger cihazlar kapali" << std::endl;
    std::cout << "  Aksam  - Tum isiklar kapali" << std::endl;
    std::cout << "  Parti  - Isiklar ve muzik acik" << std::endl;
    std::cout << "  Sinema - Isiklar kapali, TV acik" << std::endl;
    std::cout << std::endl;
    std::cout << "  DURUMLAR:" << std::endl;
    std::cout << "  Normal, Yuksek Performans, Dusuk Guc, Uyku" << std::endl;
    std::cout << "  Onceki secenegi ile durum gecmisinde gezin." << std::endl;
    std::cout << std::endl;
    std::cout << "  KRITIK CIHAZLAR:" << std::endl;
    std::cout << "  Kamera, Dedektor ve Alarm cihazlari kritiktir." << std::endl;
    std::cout << "  Bu cihazlar kapatilamaz veya kaldirilamaz." << std::endl;
    std::cout << std::endl;

    ConsoleUtils::pause();
}

void Menu::showAbout() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("HAKKINDA");

    std::cout << std::endl;
    std::cout << "  EVIM EVIM GUZEL EVIM" << std::endl;
    std::cout << "  Akilli Ev Otomasyon Sistemi" << std::endl;
    std::cout << std::endl;
    std::cout << "  Surum: 1.0.0" << std::endl;
    std::cout << "  Standart: C++98" << std::endl;
    std::cout << std::endl;
    std::cout << "  Tasarim Kaliplari:" << std::endl;
    std::cout << "  - Abstract Factory (Dedektor ailesi)" << std::endl;
    std::cout << "  - Prototype (Cihaz klonlama)" << std::endl;
    std::cout << "  - Memento (Durum gecmisi)" << std::endl;
    std::cout << "  - State (Sistem durumlari)" << std::endl;
    std::cout << "  - Observer (Bildirim sistemi)" << std::endl;
    std::cout << "  - Singleton (Logger)" << std::endl;
    std::cout << "  - Facade (SmartHome)" << std::endl;
    std::cout << "  - Adapter (Isik adaptoru)" << std::endl;
    std::cout << "  - Bridge (Cihaz implementasyonu)" << std::endl;
    std::cout << "  - Command (Menu komutlari)" << std::endl;
    std::cout << "  - Proxy (Cihaz erisim kontrolu)" << std::endl;
    std::cout << "  - Chain of Responsibility (Bildirim zinciri)" << std::endl;
    std::cout << "  - Mediator (Guvenlik koordinasyonu)" << std::endl;
    std::cout << "  - Strategy (Algilama stratejileri)" << std::endl;
    std::cout << "  - Iterator (Cihaz iterasyonu)" << std::endl;
    std::cout << std::endl;
    std::cout << "  Gelistirme Ekibi: 7 kisi" << std::endl;
    std::cout << "  (1 Entegrator + 6 Gelistirici)" << std::endl;
    std::cout << std::endl;

    ConsoleUtils::pause();
}

void Menu::shutdown() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("SISTEM KAPATMA");

    std::cout << std::endl;

    if (ConsoleUtils::getYesNoInput("Cikmak istediginizden emin misiniz?")) {
        std::cout << std::endl;
        ConsoleUtils::printInfo("Tum cihazlar kapatiliyor...");
        m_smartHome->turnAllOff();

        ConsoleUtils::printInfo("Log dosyasi kaydediliyor...");
        Logger::getInstance().info("Sistem kapatildi.");

        ConsoleUtils::printSuccess("Gule gule!");
        m_running = false;
    }
}

void Menu::listDevices() {
    std::vector<Device*> devices = m_smartHome->getAllDevices();

    if (devices.empty()) {
        ConsoleUtils::printInfo("Kayitli cihaz yok.");
        return;
    }

    std::cout << std::endl;
    std::cout << "  ID  | Tip         | Ad                  | Konum          | Durum" << std::endl;
    ConsoleUtils::printSeparator('-', 75);

    for (size_t i = 0; i < devices.size(); ++i) {
        Device* d = devices[i];

        std::ostringstream oss;
        oss << "  ";
        if (d->getId() < 10) oss << " ";
        if (d->getId() < 100) oss << " ";
        oss << d->getId() << " | ";
        std::string typeStr;
        switch (d->getType()) {
            case DEVICE_LIGHT: typeStr = "Isik"; break;
            case DEVICE_CAMERA: typeStr = "Kamera"; break;
            case DEVICE_SMOKE_DETECTOR: typeStr = "Duman Ded."; break;
            case DEVICE_GAS_DETECTOR: typeStr = "Gaz Ded."; break;
            case DEVICE_TV: typeStr = "TV"; break;
            case DEVICE_ALARM: typeStr = "Alarm"; break;
            case DEVICE_SOUND_SYSTEM: typeStr = "Ses Sis."; break;
            default: typeStr = "Bilinmiyor"; break;
        }
        oss << typeStr;
        for (size_t j = typeStr.length(); j < 11; ++j) oss << " ";
        oss << " | ";
        std::string name = d->getName();
        if (name.length() > 19) name = name.substr(0, 16) + "...";
        oss << name;
        for (size_t j = name.length(); j < 19; ++j) oss << " ";
        oss << " | ";
        std::string loc = d->getLocation();
        if (loc.length() > 14) loc = loc.substr(0, 11) + "...";
        oss << loc;
        for (size_t j = loc.length(); j < 14; ++j) oss << " ";
        oss << " | ";
        if (d->isOn()) {
            oss << "ACIK";
        } else {
            oss << "KAPALI";
        }

        if (d->isCritical()) {
            oss << " [K]";
        }

        std::cout << oss.str() << std::endl;
    }
}

void Menu::listDevicesByType(DeviceType type) {
    std::vector<Device*> devices = m_smartHome->getDevicesByType(type);

    if (devices.empty()) {
        ConsoleUtils::printInfo("Bu tipte cihaz yok.");
        return;
    }

    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << "  " << devices[i]->getInfo() << std::endl;
    }
}

char Menu::getCharInput(const std::string& prompt) {
    std::string input = ConsoleUtils::getInput(prompt);
    if (input.empty()) {
        return '\0';
    }
    return input[0];
}
void Menu::simulateSecurityEvent() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("GUVENLIK OLAYI SIMULASYONU");

    std::cout << std::endl;
    std::cout << "  Simule Edilecek Guvenlik Olayini Secin:" << std::endl;
    std::cout << "  [H] Hareket Algilandi (Kamera) - REQ 13" << std::endl;
    std::cout << "  [D] Duman Algilandi - REQ 14-16" << std::endl;
    std::cout << "  [G] Gaz Algilandi - REQ 14-16" << std::endl;
    std::cout << "  [0] Iptal" << std::endl;
    std::cout << std::endl;

    char choice = getCharInput("Seciminiz: ");

    SecurityManager* securityManager = m_smartHome->getSecurityManager();
    if (!securityManager) {
        ConsoleUtils::printError("Guvenlik Yoneticisi mevcut degil!");
        ConsoleUtils::pause();
        return;
    }

    switch (choice) {
        case 'H':
        case 'h':
            std::cout << std::endl;
            ConsoleUtils::printWarning("HAREKET ALGILAMA olayi simule ediliyor...");
            std::cout << std::endl;
            securityManager->handleMotionDetected();
            break;

        case 'D':
        case 'd':
            std::cout << std::endl;
            ConsoleUtils::printWarning("DUMAN ALGILAMA olayi simule ediliyor...");
            std::cout << std::endl;
            securityManager->handleSmokeDetected();
            break;

        case 'G':
        case 'g':
            std::cout << std::endl;
            ConsoleUtils::printWarning("GAZ ALGILAMA olayi simule ediliyor...");
            std::cout << std::endl;
            securityManager->handleGasDetected();
            break;

        case '0':
            return;

        default:
            ConsoleUtils::printError("Gecersiz secim!");
    }

    ConsoleUtils::pause();
}
void Menu::simulateDeviceFailure() {
    ConsoleUtils::clearScreen();
    ConsoleUtils::printHeader("CIHAZ ARIZASI SIMULASYONU");

    std::cout << std::endl;
    listDevices();

    if (m_smartHome->getDeviceCount() == 0) {
        ConsoleUtils::pause();
        return;
    }

    std::cout << std::endl;
    std::cout << "  Bildirim Tercihini Secin:" << std::endl;
    std::cout << "  [L] Log bildirimi" << std::endl;
    std::cout << "  [A] Alarm bildirimi" << std::endl;
    std::cout << "  [S] SMS bildirimi" << std::endl;
    std::cout << std::endl;

    char notifType = getCharInput("Bildirim tipi: ");

    switch (notifType) {
        case 'L':
        case 'l':
            m_smartHome->setNotificationPreference(NOTIFY_LOG);
            break;
        case 'A':
        case 'a':
            m_smartHome->setNotificationPreference(NOTIFY_ALARM);
            break;
        case 'S':
        case 's':
            m_smartHome->setNotificationPreference(NOTIFY_SMS);
            break;
        default:
            m_smartHome->setNotificationPreference(NOTIFY_LOG);
    }

    std::cout << std::endl;
    int id = ConsoleUtils::getIntInput("Ariza simule edilecek Cihaz ID (0=Iptal): ");

    if (id == 0) return;

    Device* device = m_smartHome->getDevice(static_cast<uint32_t>(id));
    if (device) {
        NotificationManager* notifMgr = m_smartHome->getNotificationManager();
        if (notifMgr) {
            device->addObserver(notifMgr);
        }

        std::cout << std::endl;
        ConsoleUtils::printWarning("Ariza simule ediliyor: " + device->getName());
        std::cout << std::endl;

        device->simulateFailure();

        std::cout << std::endl;
        ConsoleUtils::printInfo("Cihaz durumu degisti: HATA/PASIF");
    } else {
        ConsoleUtils::printError("Cihaz bulunamadi!");
    }

    ConsoleUtils::pause();
}

}
