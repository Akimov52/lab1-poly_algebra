
# Лабораторная работа
Выполняет команда:

**Акимов Илья**

**Гусев Дмитрий**

**Константинов Семён**

Для разработки используется Visual studio
# Задача
Разработать программную систему для выполнения алгебраических операций над полиномами от трех переменных. Пользователь должен иметь возможность сохранять полиномы в таблицах разных типов и использовать полиномы из таблиц для манипуляций. Интерфейс - консольный, возможна доработка с доьавлением графического интерфейса

# Пользовательский опыт

Пользователь взаимодействует с программой с помощью консольных команд. После запуска программы пользователю будет выведена справка со списком доступных действий.
При работе предполагается следующая последовательность действий:
Пользователь может выбрать активную таблицу, ввести полином (и его имя) для добавления в таблицы, вывести активную таблицу.
Когда в таблицах будет достаточно полиномов, пользователь может ваести выражение, содержащее имена полиномов и доступные математические операции. Будет выведен результирующий полином, пользователь может добавить его в таблицы, дав ему имя.
Также будет возможность выбрать полином по имени и вычислить его значение в произвольной точке.

# Условия/требования

 - Полиномы хранятся в виде списка.
 - Полиномы хранятся во всех таблицах одновременно. Ключом является имя. 
 - Таблиц д.б. 6 видов: линейная на массиве, линейная на списке, упорядоченная на массиве, дерево (АВЛ или красно-черное), две хэш-таблицы.
 - Операции над отдельным полиномом: вычисление в точке, умножение на константу, производная*, интеграл*.
 - Операции в выражениях из полиномов: сложение, вычитание, умножение на константу, умножение полиномов*, деление полиномов*. Операции должны выполняться, используя постфиксную форму.
 - Операции над таблицами: добавление полинома (во все сразу), удаление полинома (во всех сразу), поиск (только в активной таблице, выполняется в процессе вычисления выражений, составленных из имен полиномов).
 - Активная (выбранная пользователем) таблица должна выводиться на экран в формате, как минимум, двух столбцов: 1) имя полинома, 2) строковое представление полинома.
# ТЗ / план работы
## 1. Реализовать основной функционал алгебры полиномов
- Определить наиболее подходящую из уже сделанных реализаций работы с полиномами и взять её за основу
- Адаптировать код работы с полиномами под новые требования, добавив недостающий базовый функционал
- Покрыть код тестами
## 2. Реализовать систему хранения полиномов в таблицах
- Адаптировать существующую систему для работы с любыми из нужных контейнеров
- Реализовать требуемый набор совместимых с системой контейнеров для хранения полиномов
- Убедиться, что операции добавления, удаления, поиска полиномов в контейнерах работают, нет конфликтов имён полиномов
- Покрыть этот функционал тестами
## 3. Реализовать работу с пользователем
- Адаптировать функции парсинга полиномов
- Реализовать функции парсинга выражений с полиномами
- Реализовать исполнение пользовательских консольных команд работы с полиномами, выбор таблицы
- Тестировать разные сценарии использования, проверять edge-cases и дорабатывать программу. Поможет покрытие тестами
