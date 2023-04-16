#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <thread>
#include <time.h>

using namespace std;

const double D1 = 1.840940;
const double D2 = -1.526385;

struct F1_ {
    double x;
    double y;
};

struct F2_ {
    double x;
    double y;
};

vector<F1_> F1_1;
vector<F1_> F1_2;

vector<F2_> F2_1;
vector<F2_> F2_2;

//lg(X-0.2*Y2) - 2 * X * Y - 2 * X + 2 * Y + D1
void FILL_F1(double X, double Y, vector <F1_>& F) {
    F1_ F1__;
    double ost = log10(X - 0.2*(Y*Y)) - 2*X*Y - 2 * X + 2*Y + D1; /*Расчёт первой формулы*/
    if (ost > -0.0003 && ost < 0.0003) {
        F1__.x = X; F1__.y = Y;
        F.push_back(F1__);
    }
}

//16*Y2- (X2 - 4) * (1 - X2) + D2,
void FILL_F2(double X, double Y, vector <F2_>& F) {
    F2_ F2__;
    double ost = 16*Y*Y - (X*X - 4)*(1-X*X)+D2; /*Расчёт второй формулы*/
    if (ost > -0.0003 && ost < 0.0003) {
        F2__.x = X; F2__.y = Y;
        F.push_back(F2__);
    }
}


int main(int argc, char* argv[]) {
    setlocale(0, "");
    int VM_X = 500; int VM_Y = 500;
    if (argc > 2) {
        if (stoi(argv[1]) > 300 && stoi(argv[2]) > 300) {
            VM_X = stoi(argv[1]);
            VM_Y = stoi(argv[2]);
        }
        else VM_X = 1920; VM_Y = 1080;
    }
    else {
        VM_X = 1500; VM_Y = 1070; /*КОНСТАНТЫ РАЗМЕРНОСТИ ЭКРАНА. Без ограничений на размерность. В разумных пределах!*/
    }
    sf::RenderWindow window(sf::VideoMode(VM_X, VM_Y), "Laba8: " + to_string(VM_X) + "x" + to_string(VM_Y));

    double VERT_Y_KOEF = double(VM_Y * 10) / VM_X; /*Вычисляем высоту Y при заданном X. Используем для вычислений*/
    clock_t start_time = clock();

    /*Нижний график F1*/
    thread t1([&]() {
        for (double y = -3.5; y <= -1.5; y += 0.001) { /*Вычисляем значение функции при заданных X и Y. Если попали в заданный диапазон - значение добавляется в массив*/
            for (double x = 2.5; x >= 1.25; x -= 0.001) {
                FILL_F1(x, y, F1_1);
            }
        }
        for (double x = 1.25; x <= 10; x += 0.001) {
            for (double y = -1.25; y <= -0.8; y += 0.001) {
                FILL_F1(x, y, F1_1);
            }
        }

        for (double x = 0.9295; x >= 0; x -= 0.0005) {
            for (double y = 0.5965; y >= 0.5; y -= 0.0005) {
                FILL_F1(x, y, F1_2);
            }
        }
        for (double y = 0.5; y >= -0.5; y -= 0.0005) {
            for (double x = 0; x <= 0.5; x += 0.0005) {
                FILL_F1(x, y, F1_2);
            }
        }
        for (double y = 0.5; y <= 0.5965; y += 0.0005) {
            for (double x = 0.5; x <= 1; x += 0.0005) {
                FILL_F1(x, y, F1_2);
            }
        }
        });

    /*Итерации были написаны под мой конкретный график. У вас будут другие!*/
    /*Верхний график F1*/
    thread t2([&]() {
        for (double x = -2.2; x <= -0.4; x += 0.001) {
            for (double y = 0; y <= 0.6; y += 0.001) {
                FILL_F2(x, y, F2_1);
            }
        }
        for (double x = -0.4; x >= -2.2; x -= 0.001) {
            for (double y = 0; y >= -0.6; y -= 0.001) {
                FILL_F2(x, y, F2_1);
            }
        }
        for (double x = 0.4; x <= 2.2; x += 0.001) {
            for (double y = 0; y <= 0.6; y += 0.001) {
                FILL_F2(x, y, F2_2);
            }
        }
        for (double x = 2.2; x >= 0.4; x -= 0.001) {
            for (double y = 0; y >= -0.6; y -= 0.001) {
                FILL_F2(x, y, F2_2);
            }
        }
        });

    /*Ждём обработки всех потоков и только потом приступаем к следующим вычислениям*/
    t1.join();
    t2.join();

    double ma = 0; double my = 0;
    for (const auto& a : F1_2) {
        if (a.x > ma) {
            ma = a.x;
            my = a.y;
        }
    }
    cout << ma << " " << my << endl;

    clock_t end_time = clock();
    cout << "ДЕЛЬТА ВРЕМЕНИ " << double(end_time - start_time) / CLOCKS_PER_SEC << "с" << endl;


    sf::VertexArray F1_1_l(sf::LineStrip);
    sf::VertexArray F1_2_l(sf::LineStrip);

    sf::VertexArray F2_1_l(sf::LineStrip);
    sf::VertexArray F2_2_l(sf::LineStrip);

    /*Создаём координатную сетку*/
    /*sf::VertexArray имя(sf:LinesStrip, количество точек в векторе)*/
    sf::VertexArray koord_os(sf::LinesStrip, 5);
    koord_os[0].position = sf::Vector2f(VM_X / 2, 0);
    koord_os[1].position = sf::Vector2f(VM_X / 2, VM_Y);
    koord_os[2].position = sf::Vector2f(VM_X / 2, VM_Y / 2);
    koord_os[3].position = sf::Vector2f(0, VM_Y / 2);
    koord_os[4].position = sf::Vector2f(VM_X, VM_Y / 2);

    sf::CircleShape _str(5.f); //Стрелки вверх и вправо
    _str.setPosition(VM_X / 2 - _str.getRadius() - 0.5, 0);
    _str.setScale(1, 2);
    _str.setPointCount(3); //Уменьшаем количество сторон у круга до 3. Magic
    sf::CircleShape __str(5.f);
    __str.setPosition(VM_X, VM_Y / 2 - __str.getRadius());
    __str.setScale(1, 2);
    __str.setPointCount(3);
    __str.setRotation(90);


    /*Автоматическое создание координатных линий/точек. Отсчитывает с левого края экрана до середины с шагом в (VM_X/20)[1/20-ая часть экрана по ширине]
    Так же создаёт такой же клон, но для правой части графика.*/
    vector<sf::VertexArray> GORIZ_KOORD;
    for (int i = VM_X / 10; i < VM_X / 2; i += VM_X / 10) {
        sf::VertexArray temp(sf::LinesStrip, 2);
        temp[0].position = (sf::Vector2f(i, VM_Y / 2 + 10));
        temp[1].position = (sf::Vector2f(i, VM_Y / 2 - 10));
        GORIZ_KOORD.push_back(temp);
        temp[0].position = (sf::Vector2f(VM_X - i, VM_Y / 2 + 10));
        temp[1].position = (sf::Vector2f(VM_X - i, VM_Y / 2 - 10));
        GORIZ_KOORD.push_back(temp);
        temp.clear();
    }

    /*Создание вертикальных координатных точек. Используем то же расстояние от центра до следующей точки, как и в предыдущем методе!*/
    vector<sf::VertexArray> VERTIC_KOORD;
    for (int i = VM_X / 10; i < VM_Y / 2; i += VM_X / 10) {
        sf::VertexArray temp(sf::LinesStrip, 2);
        temp[0].position = sf::Vector2f(VM_X / 2 + 10, VM_Y / 2 + i);
        temp[1].position = sf::Vector2f(VM_X / 2 - 10, VM_Y / 2 + i);
        VERTIC_KOORD.push_back(temp);
        temp[0].position = sf::Vector2f(VM_X / 2 + 10, VM_Y / 2 - i);
        temp[1].position = sf::Vector2f(VM_X / 2 - 10, VM_Y / 2 - i);
        VERTIC_KOORD.push_back(temp);
    }

    vector<sf::Text> VERT_CH; sf::Font font; font.loadFromFile("Disket-Mono-Regular.ttf");
    for (int i = VM_X / 10, x = 1; i < VM_X / 2; i += VM_X / 10, x++) {
        sf::Text text(to_string(x), font, 25);
        text.setPosition(VM_X / 2 + i - 7, VM_Y / 2 + 10);
        VERT_CH.push_back(text);
        if (i < VM_Y / 2) {
            text.setPosition(VM_X / 2 - text.getCharacterSize() - 7, VM_Y / 2 - i - 17);
            VERT_CH.push_back(text);
        }

        text.setString(to_string(-x));
        if (i < VM_Y / 2) {
            text.setPosition(VM_X / 2 - 2 * text.getCharacterSize(), VM_Y / 2 + i - 17);
            VERT_CH.push_back(text);
        }
        text.setPosition(VM_X / 2 - i - text.getCharacterSize(), VM_Y / 2 + 10);
        VERT_CH.push_back(text);
    }


    for (int i = 0; i < F1_1.size(); i++) { 
        F1_1_l.append(sf::Vertex(sf::Vector2f(VM_X / 2 + F1_1[i].x * VM_X / 10, VM_Y / 2 - F1_1[i].y * VM_X / 10), sf::Color::Cyan));
    }
    for (int i = 0; i < F1_2.size(); i++) { 
        F1_2_l.append(sf::Vertex(sf::Vector2f(VM_X / 2 + F1_2[i].x * VM_X / 10, VM_Y / 2 - F1_2[i].y * VM_X / 10), sf::Color::Cyan));
    }

    for (int i = 0; i < F2_1.size(); i++) {
        F2_1_l.append(sf::Vertex(sf::Vector2f(VM_X / 2 + F2_1[i].x * VM_X / 10, VM_Y / 2 - F2_1[i].y * VM_X / 10), sf::Color::Green));
    }
    for (int i = 0; i < F2_2.size(); i++) {
        F2_2_l.append(sf::Vertex(sf::Vector2f(VM_X / 2 + F2_2[i].x * VM_X / 10, VM_Y / 2 - F2_2[i].y * VM_X / 10), sf::Color::Green));
    }



    bool f = true;
    window.setVerticalSyncEnabled(true);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (f)
        {
            window.clear();
            window.draw(koord_os);
            window.draw(_str);
            window.draw(__str);
            for (const auto& a : GORIZ_KOORD) {
                window.draw(a);
            }
            for (const auto& a : VERTIC_KOORD) {
                window.draw(a);
            }
            for (const auto& a : VERT_CH) {
                window.draw(a);
            }
            window.draw(F1_1_l);
            window.draw(F1_2_l);
            window.draw(F2_1_l);
            window.draw(F2_2_l);

            /*Можно настроить точность нахождения пересечений. Если поставить слишком высокую,
            то надо увеличивать точность поиска обоих графиков. Иначе будет 0 общих точек*/
            for (const auto& a : F1_1) {
                for (const auto& b : F2_2) {
                    if (abs(a.x - b.x) <= 0.002 && abs(a.y - b.y) <= 0.002) {
                        sf::CircleShape peresech(5.f);
                        peresech.setPosition(VM_X / 2 + a.x * VM_X / 10 - peresech.getRadius(), VM_Y / 2 - a.y * VM_X / 10 - peresech.getRadius());
                        peresech.setFillColor(sf::Color::Red);
                        window.draw(peresech);
                        printf("\nТочки пересечения: %f %f", a.x, a.y);
                    }
                }
            }

            f = false;
            window.display();
        }
    }
    cout << "\nПРОГРАММА ЗАВЕРШИЛАСЬ";
    return 0;
}