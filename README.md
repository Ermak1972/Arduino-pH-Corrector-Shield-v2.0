# Arduino pH Corrector Shield v 2.0
[English](https://github.com/cyberplantru/Arduino-pH-Corrector-Shield-v2.0-English)
Пример кода для автоматического управления уровнем pH.

обновление от 17/04/2014
Добавлены функции:
* Журнал расхода корректирующих жидкостей (сохранение в EEPROM)
* включение насосов в ручном режиме с счетчиком мл/сек + сохранение результата в журнал
* калибровка пропускной способности трубки
* сброс всех параметров на исходные
* сохранение всех настроеных параметров в EEPROM одним нажатием 
__________

# Описание


pH Corrector Shield это Arduino совместимая плата,
предназначенная для автоматизации контроля концентрации ионов водорода в жидкой среде

[![ScreenShot](https://raw.github.com/GabLeRoux/WebMole/master/ressources/WebMole_Youtube_Video.png)](http://www.youtube.com/watch?v=vZGudOGjutk)

Дополнительную информацию можно найти на странице проекта [link](http://www.cyberplant.info).

### Характеристики платы

![pH Correcor Shield](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_Corrector_Shield_400.jpg)

Плата использует следующие контакты Arduino:

A0 - кнопка Set
A1 - кнопка Menu
A2 - кнопка Down
A3 - кнопка Up

8, 9, 10, 11, 12, 13 - дисплей

3, 5 - tx, rx pH Circuit

6, 7 - насосы pHDown, pHUp

Питание для насосов берется с Vin Arduino

Файлы печатной платы [gerber](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_Shield_2.5.zip).

[fritzing project](http://fritzing.org/projects/ph-corrector-shield-v20)

# Сборка устройства

![pH Correcor Kit](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_kit_1_400.jpg)

Для сборки устройства понадобятся следующие элементы:
1. Cyberplant pH Correcnor Shield
2. Arduino Uno или Arduino Mega 
3. pH Circuit
4. pH Зонд
5. Блок питания 
6. Пара перистальтических насоса
7. Дисплей



# Установка библиотеки

В скетче использована библиотека для работы
с энергонезависимой памятью EEPROMex

Узнать как устанавливать библиотеки Arduino можно на сайте: <http://arduino.cc/en/Guide/Libraries>.


### Способы расширения

Плата разведена таким образом, что позволяет подключать
шилды для дистанционного управления, таки как Ethernet Shield, 
WiFi Shield, Bluetooth Shield.
В этом случае прийдется отказатся от использования дисплея, что при использовании дистанционной связи не сильно сказывается на функционале.
Так же возможно использовать вместо pH Circuit другие сенсоры и платы от Atlas Scientific, такие Conductivity Circuit и др. 
В этом случае можно значительно расширить функционал.
Кроме того на плате Arduino после установки pH Corrector Shield остаются свободные контакты, которые можно так же использовать для подключения различных сенсоров и управляющих устройств.

# FAQ

Перед использованием Arduino c этим скетчем рекомендуется
обнулить память EEPROM по этому монуалу: <http://arduino.cc/en/Tutorial/EEPROMClear>

