# UOMZ

Тестовое задание для "Уральского оптико-механического завода". Проект представляет собой встроенное программное обеспечение для платы c микроконтроллером STM32F072C8T6, датчиком напряжения батареи STC3100IST, датчиком освещённости LTR-329ALS-01 и семисегментными индикаторами. Показания датчика напряжения батареи выводятся на верхний ряд семисегментных индикаторов, а показания датчика освещённости выводятся на нижний ряд семисегментных индикаторов в реальном времени. Видеодемонстрация работы программы находится [здесь][Video_link].

<video controls muted src="video/Demo.mp4"></video>

## Требования

* Плата с микроконтроллером STM32F072C8T6, датчиком напряжения батареи STC3100IST, датчиком освещённости LTR-329ALS-01 и семисегментными индикаторами.
* [Программатор ST-LINK][ST-LINK_link].
* [Windows 10][Windows10_link].
* [STM32CubeIDE][STM32CubeIDE_link].
* [Драйвер программатора ST-LINK][ST-LINK_driver_link].

## Рабочий процесс

### Открытие проекта

Откройте STM32CubeIDE. Выберите вашу рабочую среду и нажмите "Launch".

![1_Open_project_text][1_Open_project_link]

![2_Open_project_text][2_Open_project_link]

Выберите "File" -> "Import...".

![3_Open_project_text][3_Open_project_link]

Выберите "General" -> "Existing Projects into Workspace" и нажмите "Next >".

![4_Open_project_text][4_Open_project_link]

Выберите "Select root directory:" -> "Browse..." и укажите путь к папке с проектом. Нажмите "Finish".

![5_Open_project_text][5_Open_project_link]

Проект открыт успешно.

![6_Open_project_text][6_Open_project_link]

### Сборка

Выберите "Project" -> "Build All".

![7_Build_text][7_Build_link]

Проект собран успешно.

![8_Build_text][8_Build_link]

### Загрузка программы в микроконтроллер

Выберите "Run" -> "Run".

![9_Run_text][9_Run_link]

### Отладка

Выберите "Run" -> "Debug".

![10_Debug_text][10_Debug_link]




[Video_link]: video/Demo.mp4
[ST-LINK_link]: https://www.st.com/en/development-tools/st-link-v2.html
[Windows10_link]: https://www.microsoft.com/software-download/windows10
[STM32CubeIDE_link]: https://www.st.com/en/development-tools/stm32cubeide.html
[ST-LINK_driver_link]: https://www.st.com/en/development-tools/stsw-link009.html#get-software

[1_Open_project_link]: images/1_Open_project.png
[2_Open_project_link]: images/2_Open_project.png
[3_Open_project_link]: images/3_Open_project.png
[4_Open_project_link]: images/4_Open_project.png
[5_Open_project_link]: images/5_Open_project.png
[6_Open_project_link]: images/6_Open_project.png
[7_Build_link]: images/7_Build.png
[8_Build_link]: images/8_Build.png
[9_Run_link]: images/9_Run.png
[10_Debug_link]: images/10_Debug.png