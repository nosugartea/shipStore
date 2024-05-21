-----------------------------------------------------------------
---------------------Начальное состояние-------------------------
-----------------------------ПР1---------------------------------
-----------------------------------------------------------------

-----------------------------------------------------------------
---------------------------INS_EI--------------------------------
-----------------------------------------------------------------
/*SELECT * FROM INS_EI('0', NULL, NULL);
SELECT * FROM INS_EI('003', 'мм', 'Миллиметр');
SELECT * FROM INS_EI('006', 'м', 'Метр');
SELECT * FROM INS_EI('055', 'м2', 'Квадратный метр');
SELECT * FROM EI;*/

-----------------------------------------------------------------
-----------------------ADD_CHEM_CLASS----------------------------
-----------------------------------------------------------------
/*SELECT * FROM ADD_CHEM_CLASS(NULL, 'Изделие', 2, NULL); /*1*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Суда', 2, 1); /*2*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Транспортные морские', 2, 2); /*3*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Пассажирские самоходные', 2, 3); /*4*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Пассажирские водоизмещающие', 2, 4); /*5*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Грузопассажирские', 2, 4); /*6*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Сухогрузные самоходные', 2, 3); /*7*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Общего назничания', 2, 7); /*8*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Для навалочных грузов', 2, 7); /*9*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Промысловые', 2, 2); /*10*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Добывающие морские самоходные', 2, 10); /*11*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Траулеры', 2, 11); /*12*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Сейнеры', 2, 11); /*13*/

SELECT * FROM ADD_CHEM_CLASS(NULL, 'Обрабатывающие морские исходные', 2, 10); /*14*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Рыбообрабатывающие', 2, 14); /*15*/
SELECT * FROM ADD_CHEM_CLASS(NULL, 'Китообрабатывающие', 2, 14); /*16*/
SELECT * FROM CHEM_CLASS;*/

/*SELECT * FROM ADD_PROD(NULL, 'Сейнер МРС-225', 13);
SELECT * FROM ADD_PROD(NULL, 'Сейнер МРС-150', 13);
SELECT * FROM ADD_PROD(NULL,'ТРАУЛЕР RUBICON 1800', 12);
SELECT * FROM ADD_PROD(NULL,'СРТМ58 "Азимут"', 15);
SELECT * FROM ADD_PROD(NULL,'MERIDA 1700', 16);*/


-----------------------------------------------------------------
---------------------Добавление и тестирование-------------------
-----------------------------ПР2---------------------------------
-----------------------------------------------------------------

-----------------------------------------------------------------
-----------------------ADD_CHEM_CLASS----------------------------
-----------------------------------------------------------------
/*SELECT * FROM ADD_CHEM_CLASS('КлПО', 'Класс ПО', 2, NULL); /*17*/

SELECT * FROM ADD_CHEM_CLASS('Изделие', 'Изделие', 2, 17); /*18*/
SELECT * FROM CHANGE_PARENT_CHEM_CLASS(2, 18);
SELECT * FROM DELETE_CHEM_CLASS(1);

SELECT * FROM ADD_CHEM_CLASS('Enum', 'Перечисление', 2, 17); /*19*/

SELECT * FROM ADD_CHEM_CLASS('ПерСтр', 'Перечисление строк', 2, 19); /*20*/
SELECT * FROM ADD_CHEM_CLASS('ПерИзобр', 'Перечисление изображений', 2, 19); /*21*/
SELECT * FROM ADD_CHEM_CLASS('ЧислПер', 'Численное перечисление', 2, 19); /*22*/

SELECT * FROM ADD_CHEM_CLASS('ПерВещ', 'Перечисление вещ', 2, 22); /*23*/
SELECT * FROM ADD_CHEM_CLASS('ПерЦел', 'Перечисление целых', 2, 22); /*24*/
SELECT * FROM CHEM_CLASS;*/

-----------------------------------------------------------------
------------------------INS_VAL_ENUM-----------------------------
-----------------------------------------------------------------
--Сварное соединение
--Заклёпочное соединение
--Резьбовое соединение
--Вид Килевая линия : морская, речная, с конструктивным дифферентом, с реданом, криволинейная

/*SELECT * FROM ADD_CHEM_CLASS('ТипМат', 'Тип материала', 2, 20); /*25*/
SELECT * FROM ADD_CHEM_CLASS('ТипКил', 'Тип килевой линии', 2, 20); /*26*/
SELECT * FROM CHEM_CLASS;*/
/*
SELECT * FROM INS_VAL_ENUM(25, '1', 'Сталь', NULL, NULL, NULL);
SELECT * FROM INS_VAL_ENUM(25, '2', 'Алюминиево-магниевый сплав', NULL, NULL, NULL);
SELECT * FROM INS_VAL_ENUM(25, '3', 'Пластмасса', NULL, NULL, NULL);
SELECT * FROM INS_VAL_ENUM(25, '4', 'Дерево', NULL, NULL, NULL);
SELECT * FROM INS_VAL_ENUM(25, '5', 'Железобетон', NULL, NULL, NULL);
SELECT * FROM POS_ENUM;

SELECT * FROM INS_VAL_ENUM(26, 'A', 'Криволинейная', NULL, NULL, 'https://drive.google.com/file/d/1B5MkYiNZqO3SF_L3lqM7w5gYZRIxmw3D/view?usp=sharing');
SELECT * FROM INS_VAL_ENUM(26, 'B', 'С конструктивным дифферентом', NULL, NULL, 'https://drive.google.com/file/d/18Pz9h2Eev2VtiKuRWWPnAwfuZTLQcxEX/view?usp=sharing');
SELECT * FROM INS_VAL_ENUM(26, 'C', 'С реданом', NULL, NULL, 'https://drive.google.com/file/d/1n-R2b_RBXht_FXdhtRM8FQC1HDkqBneT/view?usp=sharing');
SELECT * FROM POS_ENUM;*/

/*SELECT * FROM DEL_ENUM_VAL(1);
SELECT * FROM POS_ENUM;*/

-----------------------------------------------------------------
-----------------------------UP_VAL------------------------------
-----------------------------------------------------------------
/*CALL UP_VAL(3);
SELECT * FROM POS_ENUM;*/

-----------------------------------------------------------------
----------------------------DOWN_VAL-----------------------------
-----------------------------------------------------------------
/*CALL DOWN_VAL(3);
SELECT * FROM POS_ENUM;*/

-----------------------------------------------------------------
-------------------------FIND_LIST_ENUM--------------------------
-----------------------------------------------------------------
--SELECT * FROM FIND_LIST_ENUM(25);


-----------------------------------------------------------------
----------------------------DEL_ENUM-----------------------------
-----------------------------------------------------------------
--SELECT * FROM DEL_ENUM(28);

-----------------------------------------------------------------
---------------------Добавление и тестирование-------------------
-----------------------------ПР3---------------------------------
-----------------------------------------------------------------
--SELECT * FROM ADD_CHEM_CLASS('ОсРаз','Основные размерения', 2, 20); /* 27*/

/*SELECT * FROM INS_PARAMETR('L', 'Длина', 2, 23); /*id_par = 1*/
SELECT * FROM INS_PARAMETR('W', 'Ширина', 2, 23); /*id_par = 2*/
SELECT * FROM INS_PARAMETR('V', 'Осадка', 2, 23); /*id_par = 3*/
SELECT * FROM INS_PARAMETR('SH', 'Высота борта', 2, 23); /*id_par = 4*/
SELECT * FROM INS_PARAMETR('M', 'Материал', 1, 25); /*id_par = 5*/
SELECT * FROM INS_PARAMETR('KL', 'Килевая линия', 1, 26); /*id_par = 6*/

SELECT * FROM ADD_PARAMETR_CLASS(1, 13, 20, 50);
SELECT * FROM ADD_PARAMETR_CLASS(2, 13, 5, 10);
SELECT * FROM ADD_PARAMETR_CLASS(3, 13, 1, 5);
SELECT * FROM ADD_PARAMETR_CLASS(4, 13, 10, 20);
SELECT * FROM ADD_PARAMETR_CLASS(5, 13, NULL, NULL);
SELECT * FROM ADD_PARAMETR_CLASS(6, 13, NULL, NULL);*/

/*SELECT * FROM ADD_PROD(NULL, 'Сейнер МРС-225', 13);
SELECT * FROM ADD_PROD(NULL, 'Сейнер МРС-150', 13);*/

/*SELECT * FROM FIND_PAR_PROD(1, 13);
SELECT * FROM WRITE_PAR_PROD(1, 1, 23, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(1, 2, 7.3, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(1, 3, 4.2, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(1, 4, 12.9, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(1, 5, NULL, NULL, 2, NULL);
SELECT * FROM WRITE_PAR_PROD(1, 6, NULL, NULL, 8, NULL);
SELECT * FROM FIND_PAR_PROD(1, 13);*/

-----------------------------------------------------------------
------------------------------ZASHITA----------------------------
-----------------------------------------------------------------
/*SELECT * FROM ADD_PROD(NULL,'ТРАУЛЕР RUBICON 1800', 12);
SELECT * FROM ADD_PARAMETR_CLASS(1, 12, 5, 10);
SELECT * FROM ADD_PARAMETR_CLASS(2, 12, 2, 4);
SELECT * FROM ADD_PARAMETR_CLASS(3, 12, 1, 2);
SELECT * FROM ADD_PARAMETR_CLASS(4, 12, 3, 7);
SELECT * FROM ADD_PARAMETR_CLASS(5, 12, NULL, NULL);
SELECT * FROM ADD_PARAMETR_CLASS(6, 12, NULL, NULL);

SELECT * FROM WRITE_PAR_PROD(3, 1, 7.69, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(3, 2, 2.85, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(3, 3, 1.13, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(3, 4, 4.3, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(3, 5, NULL, NULL, 3, NULL);
SELECT * FROM WRITE_PAR_PROD(3, 6, NULL, NULL, 8, NULL);
SELECT * FROM FIND_PAR_PROD(3, 12);

SELECT * FROM ADD_PROD(NULL, 'Ведущий', 16); -- китобойное судно
SELECT * FROM ADD_PARAMETR_CLASS(1, 16, 50, 80);
SELECT * FROM ADD_PARAMETR_CLASS(2, 16, 7, 15);
SELECT * FROM ADD_PARAMETR_CLASS(3, 16, 4, 5);
SELECT * FROM ADD_PARAMETR_CLASS(4, 16, 10, 20);
SELECT * FROM ADD_PARAMETR_CLASS(5, 16, NULL, NULL);
SELECT * FROM ADD_PARAMETR_CLASS(6, 16, NULL, NULL);

SELECT * FROM WRITE_PAR_PROD(4, 1, 63.6, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(4, 2, 9.5, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(4, 3, 4.4, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(4, 4, 14.3, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(4, 5, NULL, NULL, 2, NULL);
SELECT * FROM WRITE_PAR_PROD(4, 6, NULL, NULL, 6, NULL);*/
SELECT * FROM FIND_PAR_PROD(2, 13);

SELECT * FROM WRITE_PAR_PROD(2, 1, 23, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(2, 2, 5.85, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(2, 3, 2.13, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(2, 4, 12.3, NULL, NULL, NULL);
SELECT * FROM WRITE_PAR_PROD(2, 5, NULL, NULL, 3, NULL);
SELECT * FROM WRITE_PAR_PROD(2, 6, NULL, NULL, 8, NULL);

SELECT * FROM ADD_PARAMETR_CLASS(1, 14, 50, 250);
SELECT * FROM ADD_PARAMETR_CLASS(2, 14, 7, 30);
SELECT * FROM ADD_PARAMETR_CLASS(3, 14, 4, 10);
SELECT * FROM ADD_PARAMETR_CLASS(4, 14, 10, 20);
SELECT * FROM ADD_PARAMETR_CLASS(5, 14, NULL, NULL);
SELECT * FROM ADD_PARAMETR_CLASS(6, 14, NULL, NULL);

SELECT * FROM COPY_PAR(14, 15);

SELECT * FROM ADD_PROD(NULL, 'Восток', 15);
SELECT * FROM COPY_PROD_PAR(4, 5);
SELECT * FROM COPY_PAR_CLASS(14, 15);
SELECT * FROM FIND_PAR_PROD(5, 15);

SELECT * FROM SEARCH_PRODUCTS_BY_PARAMETER('Длина', 0, 50);

SELECT * FROM FIND_PROD_BY_CLASS('Добывающие морские самоходные');

SELECT * FROM PARAMETR1;
SELECT * FROM PAR_CLASS1 ORDER BY ID_CLASS, ID_PAR;
SELECT * FROM PAR_PROD1 ORDER BY ID_PROD, ID_PAR;
SELECT * FROM EI;
SELECT * FROM CHEM_CLASS;
SELECT * FROM PROD;
SELECT * FROM POS_ENUM;

--SELECT setval('PROD_GEN', 4, false); -- для сброса генератора (спросить у Агнии)