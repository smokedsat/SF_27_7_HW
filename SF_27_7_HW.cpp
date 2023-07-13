#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <string>

class Logger {
public:
    Logger(const std::string& filename)
        : file(filename), mutex() {}

    ~Logger() {
        file.close();
    }

    void writeLog(const std::string& message) {
        std::unique_lock<std::shared_mutex> lock(mutex, std::try_to_lock);
        if (lock.owns_lock()) {
            file << message << std::endl;
        }
        else {
            // Обработка ситуации, когда не удалось захватить блокировку
            // Например, можно выбросить исключение или проигнорировать запись
        }
    }

    std::string readLog() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        std::string line;
        std::getline(file, line);
        return line;
    }

private:
    std::fstream file;
    std::shared_mutex mutex;
};