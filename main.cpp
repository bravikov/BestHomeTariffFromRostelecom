/* Программа для расчета наиболее выгодного тарифа местной связи Ростелекома
 *
 * Сборка и запуск:
 *     g++ -std=c++11 -lm main.cpp; ./a.out
 * */

#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using std::abs;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::min_element;

class Tariff
{
public:
    Tariff(const char * name,
          double freeMinutes,
          double moneyPerMonth,
          double moneyPerMinute):
        m_name(name),
        m_freeMinutes(freeMinutes),
        m_moneyPerMonth(moneyPerMonth),
        m_moneyPerMinute(moneyPerMinute) {}

    static void setSubscriberLineCharge(const double money);
    static double getSubscriberLineCharge();

    /* Устанавливает количество минут в статической приватной переменной,
     * для поддержки работы оператора сравнения */
    static void setMinutes(const double minutes);

    const char * name();

    double price(const double minutes);

    friend bool operator<(Tariff & left, Tariff & right);

private:
    static double m_subscriberLineCharge;
    static double m_minutes;

    const char * m_name;

    const double m_freeMinutes;
    const double m_moneyPerMonth;
    const double m_moneyPerMinute;
};

////////////////////////////////////////////////////////////////////////////////
int main()
{
    // Стоимость предоставление в постоянное пользование абонентской линии
    Tariff::setSubscriberLineCharge(180);

    vector<Tariff> tariffs{
        Tariff("Абонентский",       0, 210, 0.00),
        Tariff("Комбинированный", 280, 120, 0.36),
        Tariff("Дополнительный",  120,  32, 0.50),
        Tariff("Повременный",       0,   0, 0.46)
    };

    const int NumberOfMinutes = 700;

    auto firstTariff = tariffs.begin();
    auto lastTariff = tariffs.end();

    cout << "Выгодный тариф: " << endl;

    for (int m = 0; m < NumberOfMinutes; m++) {
        static vector<Tariff>::iterator minTariff;

        Tariff::setMinutes(m);

        auto tariff = min_element(firstTariff, lastTariff);

        if (minTariff != tariff) {
            minTariff = tariff;
            cout << "От " << m << " минут - " << minTariff->name() << endl;
        }
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////

bool operator<(Tariff & left, Tariff & right)
{
    return (
            left.price(left.m_minutes) < right.price(right.m_minutes)
    ) ? true : false;
}

double Tariff::m_subscriberLineCharge = 0;
double Tariff::m_minutes = 0;

void Tariff::setMinutes(const double minutes)
{
    m_minutes = minutes;
}

const char * Tariff::name()
{
    return m_name;
}

void Tariff::setSubscriberLineCharge(const double money)
{
    m_subscriberLineCharge = money;
}

double Tariff::getSubscriberLineCharge()
{
    return m_subscriberLineCharge;
}

double Tariff::price(const double minutes)
{
    const double P = m_subscriberLineCharge;
    const double F = m_freeMinutes;
    const double A = m_moneyPerMonth;
    const double C = m_moneyPerMinute;
    const double t = minutes;

    return A + P + (C / 2) * (t + abs(t - F) - F);
}
