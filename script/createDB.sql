-----------------------------------------------------------------------------
--------------------------- Создание таблицы EI -----------------------------
-----------------------------------------------------------------------------
-- Единицы измерения
CREATE TABLE EI (
    ID_EI       INTEGER NOT NULL,
    SHORT_NAME  VARCHAR(10),
    NAME        VARCHAR(50)
);

-- Добавление поля (неизвестно зачем)
alter TABLE EI
ADD CODE varchar(15);

-- Объявление первичного ключа 
ALTER TABLE EI 
ADD PRIMARY KEY (ID_EI);

-- Создание последователноти для генерации уникальных значений/идентификаторов
CREATE SEQUENCE EI_GEN;

-- Функция: возвращает новый уникальный идентификатор ONEW
CREATE OR REPLACE FUNCTION GEN_ID_EI()
RETURNS INTEGER AS $$
-- DECLARE
--   ONEW INTEGER;
BEGIN
  -- ONEW := nextval('EI_GEN'); -- Вызов последовательности
  -- RETURN ONEW;
  RETURN nextval('EI_GEN');
END;
$$ LANGUAGE plpgsql;
/*===============================
Строчка $$ LANGUAGE plpgsql; 
в конце функции является необходимой для PostgreSQL, особенно если вы используете PL/pgSQL, 
который является расширением языка SQL, которое поддерживается PostgreSQL. 
PL/pgSQL позволяет писать хранимые процедуры, триггеры, 
функции и другие сложные объекты базы данных с использованием полноценного языка программирования.

Ключевое слово LANGUAGE указывает язык, в котором написан код функции. 
В данном случае plpgsql означает, что функция написана на языке PL/pgSQL.

Эта строчка является обязательной, потому что без нее PostgreSQL не сможет понять, 
какой язык используется для написания функции, и, следовательно, не сможет правильно интерпретировать и выполнить код функции.
===============================*/

-- Пример вызова генерации
-- SELECT GEN_ID_EI() INTO vId;
/*=============================
Здесь vId должна - переменная,
в которую сохраняется возвращенное значение.
==============================*/

-----------------------------------------------------------------------------
----------------------- Создание таблицы CHEM_CLASS -------------------------
-----------------------------------------------------------------------------
-- Классы изделия
CREATE TABLE CHEM_CLASS (
    SHORT_NAME  VARCHAR(10),
    ID_CLASS    INTEGER NOT NULL,
    NAME        VARCHAR(50),
    MAIN_CLASS  INTEGER,
    BASE_EI     INTEGER
);

-- Первичный ключ
ALTER TABLE CHEM_CLASS 
ADD PRIMARY KEY (ID_CLASS);

-- Внешний ключ для указания единицы измерения количества
ALTER TABLE CHEM_CLASS 
ADD FOREIGN KEY (BASE_EI) REFERENCES EI (ID_EI);

-- Внешний ключ для указания отношения конкретизации   
ALTER TABLE CHEM_CLASS 
ADD FOREIGN KEY (MAIN_CLASS) REFERENCES CHEM_CLASS (ID_CLASS);

-- Генератор для классификатора
CREATE SEQUENCE CHEM_CLASS_GEN;

-- Процедура для вызова генератора Классификатора
CREATE OR REPLACE FUNCTION GEN_ID_CLASS()
RETURNS INTEGER AS $$
BEGIN
  RETURN nextval('CHEM_CLASS_GEN');
END;
$$ LANGUAGE plpgsql;
/*===========================================
Чтобы изменить инкремент уже существующей последовательности, 
можно использовать команду ALTER SEQUENCE:
	ALTER SEQUENCE CHEM_CLASS_GEN
	INCREMENT BY 5;
После выполнения этой команды, 
каждый следующий вызов nextval('CHEM_CLASS_GEN') 
вернет значение, которое на 5 больше предыдущего.
=============================================*/

-- Пример вызова генератора   
-- SELECT GEN_ID_CLASS() INTO vId;

-----------------------------------------------------------------------------
-------------------------- Создание таблицы PROD ----------------------------
-----------------------------------------------------------------------------
-- Продукция (Изделия)
CREATE TABLE PROD (
    ID_PROD     INTEGER NOT NULL,
    SHORT_NAME  VARCHAR(50),
    NAME        VARCHAR(250),
    ID_CLASS    INTEGER NOT NULL
);

ALTER TABLE PROD 
ADD PRIMARY KEY (ID_PROD);

-- Объявление внешнего ключа для указания класса изделия
ALTER TABLE PROD 
ADD FOREIGN KEY (ID_CLASS) REFERENCES CHEM_CLASS (ID_CLASS);

-- Объявление генератора
CREATE SEQUENCE PROD_GEN;

CREATE OR REPLACE FUNCTION GEN_ID_PROD()
RETURNS INTEGER AS $$
BEGIN
  RETURN nextval('PROD_GEN');
END;
$$ LANGUAGE plpgsql;
-- Пример вызова генератора   
-- SELECT GEN_ID_PROD() INTO vId;

-----------------------------------------------------------------------------
-------------------------- Создание таблицы ENUM ----------------------------
-----------------------------------------------------------------------------
CREATE TABLE POS_ENUM (
    ID_POS     SERIAL PRIMARY KEY,   -- Код значения
    ID_ENUM    INTEGER NOT NULL,     -- Ид. перечисления
    NUM        INTEGER NOT NULL,     -- Порядковый номер значения
    SHORT_NAME VARCHAR(15),          -- Обозначение значения
    E_NAME     VARCHAR(50),          -- Имя значения
    RVAL       DOUBLE PRECISION,     -- Вещественное значение
    INT_VAL    INTEGER,              -- Целое значение
    PIC_VAL    VARCHAR(150)          -- Путь к файлу картинки
);

-- Внешний ключ
ALTER TABLE POS_ENUM  
ADD FOREIGN KEY (ID_ENUM) 
REFERENCES CHEM_CLASS(ID_CLASS);

-- Индекс
CREATE UNIQUE INDEX POS_ENUM_I ON POS_ENUM (ID_ENUM, NUM);

-- Создание последовательности
CREATE SEQUENCE POS_ENUM_GEN;

-- Создание функции для вызова последовательности
CREATE OR REPLACE FUNCTION GEN_POS_ENUM() 
RETURNS INTEGER AS $$
DECLARE
BEGIN
  RETURN nextval('POS_ENUM_GEN');
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
------------------------ Создание таблицы PARAMETR1 -------------------------
-----------------------------------------------------------------------------
CREATE TABLE PARAMETR1 (
    ID_PAR      SERIAL PRIMARY KEY,            -- Уникальный идентификатор параметра
    SHORT_NAME  VARCHAR(15),                   -- Краткое название параметра
    NAME        VARCHAR(100),                  -- Полное название параметра
    EI_PAR      INTEGER REFERENCES EI(ID_EI),  -- Идентификатор единицы измерения параметра
    TYPE_P      INTEGER NOT NULL REFERENCES CHEM_CLASS(ID_CLASS), 
											   -- Идентификатор типа параметра 
	MIN_VAL  DOUBLE PRECISION,        -- Минимальное значение параметра
    MAX_VAL  DOUBLE PRECISION         -- Максимальное значение параметра
);

CREATE SEQUENCE PAR_GEN;

CREATE OR REPLACE FUNCTION GEN_ID_PAR()
RETURNS INTEGER AS $$
BEGIN
    RETURN nextval('PAR_GEN');
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
------------------------ Создание таблицы PAR_CLASS1 ------------------------
-----------------------------------------------------------------------------
CREATE TABLE PAR_CLASS1 (
	ID_CLASS INTEGER NOT NULL,
    ID_PAR   INTEGER NOT NULL,
	NUM 	 INTEGER,
	MIN_VAL  DOUBLE PRECISION,        -- 
    MAX_VAL  DOUBLE PRECISION,        --
    PRIMARY KEY (ID_CLASS, ID_PAR),
    FOREIGN KEY (ID_CLASS) REFERENCES CHEM_CLASS(ID_CLASS),
    FOREIGN KEY (ID_PAR) REFERENCES PARAMETR1(ID_PAR)
);

-----------------------------------------------------------------------------
------------------------ Создание таблицы PAR_PROD1 -------------------------
-----------------------------------------------------------------------------
CREATE TABLE PAR_PROD1 (
    ID_PROD   INTEGER NOT NULL,        -- Идентификатор продукта
    ID_PAR    INTEGER NOT NULL,        -- Идентификатор параметра  
    VAL_INT   INTEGER,                 -- Целочисленное значение параметра
	VAL_R     DOUBLE PRECISION,        -- Рациональнное значение параметра
    VAL_STR   VARCHAR(150),            -- Текстовое представление параметра
    ENUM_VAL  INTEGER  NULL, 		   -- Идентификатор перечисления
    NOTE      VARCHAR(200),            -- Примечание к параметру
    PRIMARY KEY (ID_PROD, ID_PAR),
    FOREIGN KEY (ID_PROD) REFERENCES PROD(ID_PROD),
    FOREIGN KEY (ID_PAR) REFERENCES PARAMETR1(ID_PAR),
	FOREIGN KEY (ENUM_VAL) REFERENCES POS_ENUM(ID_POS)
);
