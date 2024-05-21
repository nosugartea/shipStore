CREATE OR REPLACE FUNCTION SEARCH_PRODUCTS_BY_PARAMETER (
	pName VARCHAR(100),
	pMinVal DOUBLE PRECISION,
	pMaxVal DOUBLE PRECISION
)
RETURNS TABLE (
	vIdProd INTEGER,
	vName VARCHAR(100),
	vValI INTEGER,
	vValR DOUBLE PRECISION
) AS $$
BEGIN
	RETURN QUERY
	SELECT p.ID_PROD, p.NAME, pp.VAL_INT, pp.VAL_R
	FROM PROD p
	INNER JOIN PAR_PROD1 pp ON p.ID_PROD = pp.ID_PROD
	INNER JOIN PARAMETR1 pr ON pp.ID_PAR = pr.ID_PAR
	WHERE pr.NAME = pName AND 
	((pp.VAL_INT IS NOT NULL AND pp.VAL_INT BETWEEN pMinVal AND pMaxVal) OR
	((pp.VAL_R IS NOT NULL AND pp.VAL_R BETWEEN pMinVal AND pMaxVal)));
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Копирует параметры одного родительского класса классу -----
-------- потомку ------------------------------------------------------------
-------- вход: pIdMain - ид. родительского класса, pIdClass - ид потомка ----
-------- выход: oRes - 0 - ошибка, 1 - ОК -----------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION COPY_PAR (
	pIdMain INTEGER, 
	pIdClass INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vYesMain INTEGER;
    vPar INTEGER;
    vYesPar INTEGER;
BEGIN
	-- проверка наличия потомка
    SELECT oYes INTO vYesMain FROM IN_GR(pIdMain, pIdClass);
    oRes := 0;
    
    IF (vYesMain > 0) THEN
        FOR vPar IN 
		SELECT p1.ID_PAR FROM PAR_CLASS1 p1 WHERE p1.ID_CLASS = pIdMain LOOP
            SELECT COUNT(*) INTO vYesPar FROM PAR_CLASS1 p2 WHERE p2.ID_PAR = vPar AND p2.ID_CLASS = pIdClass;
            IF (vYesPar = 0) THEN
                INSERT INTO PAR_CLASS1(ID_CLASS, ID_PAR) VALUES (pIdClass, vPar);
				IF NOT FOUND THEN
            		oRes := 0;
       			ELSE
            		oRes := 1;
        		END IF;
            END IF;
        END LOOP;
    END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION COPY_PROD_PAR (
	pIdMain INTEGER, 
	pIdClass INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vYesMain INTEGER;
    vPar INTEGER;
    vYesPar INTEGER;
	vValInt INTEGER;
	vValR DOUBLE PRECISION;
	vValStr VARCHAR(150);
	vEnVal INTEGER;
BEGIN
	-- проверка наличия потомка
    SELECT oYes INTO vYesMain FROM IN_GR(pIdMain, pIdClass);
    oRes := 0;
    
    IF (vYesMain > 0) THEN
        FOR vPar IN 
		SELECT p1.ID_PAR FROM PAR_PROD1 p1 WHERE p1.ID_PROD = pIdMain LOOP
            SELECT COUNT(*) INTO vYesPar FROM PAR_PROD1 p2 WHERE p2.ID_PAR = vPar AND p2.ID_PROD = pIdClass;
            IF (vYesPar != 0) THEN
				SELECT p2.VAL_INT, p2.VAL_R, p2.VAL_STR, p2.ENUM_VAL INTO vValInt, vValR, vValStr, vEnVal
				FROM PAR_PROD1 p2 WHERE p2.ID_PAR = vPar AND p2.ID_PROD = pIdMain;
				
                UPDATE PAR_PROD1 SET VAL_INT = vValInt, VAL_R = vValR, VAL_STR = vValStr, ENUM_VAL = vEnVal
				WHERE ID_PROD = pIdClass AND ID_PAR = vPar;
				
				IF NOT FOUND THEN
            		oRes := 0;
       			ELSE
            		oRes := 1;
        		END IF;
            END IF;
        END LOOP;
    END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION COPY_PAR_CLASS (
	pIdMain INTEGER, 
	pIdClass INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vYesMain INTEGER;
    vPar INTEGER;
    vYesPar INTEGER;
	vNum INTEGER;
	vMinVal DOUBLE PRECISION;
	vMaxVal DOUBLE PRECISION;
BEGIN
	-- проверка наличия потомка
    SELECT oYes INTO vYesMain FROM IN_GR(pIdMain, pIdClass);
    oRes := 0;
    
    IF (vYesMain > 0) THEN
        FOR vPar IN 
		SELECT p1.ID_PAR FROM PAR_CLASS1 p1 WHERE p1.ID_CLASS = pIdMain LOOP
            SELECT COUNT(*) INTO vYesPar FROM PAR_CLASS1 p2 WHERE p2.ID_PAR = vPar AND p2.ID_CLASS = pIdClass;
            IF (vYesPar != 0) THEN
				SELECT p2.NUM, p2.MIN_VAL, p2.MAX_VAL INTO vNum, vMinVal, vMaxVal
				FROM PAR_CLASS1 p2 WHERE p2.ID_PAR = vPar AND p2.ID_CLASS = pIdMain;
				
                UPDATE PAR_CLASS1 SET NUM = vNum, MIN_VAL = vMinVal, MAX_VAL = vMaxVal
				WHERE ID_CLASS = pIdClass AND ID_PAR = vPar;
				
				IF NOT FOUND THEN
            		oRes := 0;
       			ELSE
            		oRes := 1;
        		END IF;
            END IF;
        END LOOP;
    END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Создает новый экземпляр продукции -------------------------
-------- потомку ------------------------------------------------------------
-------- вход: pIdClass - класс продукта, pShName - обозначение продукта ----
-------- pName - имя продукта -----------------------------------------------
-------- выход: oRes - 0 - ошибка, 1 - ОК -----------------------------------
-------- эффекты: проверяем если заданный класс - лист, создаем -------------
-------- запись продукта; возвращаем код положительного результата, если ----
-------- создалась пара в списках значений параметров -----------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION INS_PROD (
	pIdClass INTEGER, 
	pShName VARCHAR(50), 
	pName VARCHAR(250),
	OUT oIdProd INTEGER,
	OUT oRes INTEGER
)
RETURNS RECORD AS $$
DECLARE 
    vYesClass INTEGER;
    vYesTerm INTEGER;
BEGIN
    oRes := 0; oIdProd = NULL;
    SELECT COUNT(*) INTO vYesClass -- ищем классы с заданным ид.
	FROM CHEM_CLASS WHERE ID_CLASS = pIdClass;
    
    IF (vYesClass > 0) THEN
        SELECT COUNT(*) INTO vYesTerm -- ищем классы наследуемые от заданного класса
		FROM CHEM_CLASS WHERE MAIN_CLASS = pIdClass;
        -- если заданный класс - лист, создаем запись продукта
		IF (vYesTerm = 0) THEN
            oRes := 1;
            SELECT NEXTVAL('GEN_ID_PROD') INTO oIdProd;
            INSERT INTO PROD (ID_PROD, SHORT_NAME, NAME, ID_CLASS) 
            VALUES (oIdProd, pShName, pName, pIdClass); 
            
			-- смотрим создалась ли пара в PAR_PROD1
            oRes := (SELECT oRes FROM COPY_PAR_PROD(oIdProd));
        END IF;
    END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Формирует список для значений параметров экземпляра -------
-------- pIdProd ------------------------------------------------------------
-------- вход: pIdProd - ид. продукта ---------------------------------------
-------- выход: oRes - 0 - ошибка, 1 - ОК -----------------------------------
-------- эффекты: проверяем существует ли пара (изделие, параметр) ----------
-------- в списках значений параметров, если нет, то создаем ----------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION COPY_PAR_PROD (
	pIdProd INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vPar INTEGER;
    vIdClass INTEGER;
    vYes INTEGER;
BEGIN
    oRes := 0;
    vYes := 1;
    vPar := 0;
    
	-- определяем к какому классу относится PROD
    SELECT ID_CLASS INTO vIdClass 
	FROM PROD WHERE ID_PROD = pIdProd;
    
    FOR vPar IN 
	SELECT p1.PAR FROM PAR_CLASS1 p1 
	WHERE p1.CLASS = vIdClass 
	LOOP
		-- смотрим существует ли пара (изделие, параметр)
		-- в списках значений параметров, если нет, то создаем
        SELECT COUNT(*) INTO vYes FROM PAR_PROD1 
		WHERE ID_PROD = pIdProd AND ID_PAR = vPar;
        IF (vYes = 0) THEN
            INSERT INTO PAR_PROD1 (ID_PROD, ID_PAR) VALUES (pIdProd, vPar);
            oRes := 1;
        ELSE
            oRes := 0;
        END IF;
    END LOOP;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Создает новый параметр и записывает его в каталог ---------
-------- вход: pShName - обозначение параметра, pName - имя параметра -------
-------- pIdEI - ид. единицы измерения, pTypePar - ид. типа параметра -------
-------- выход: oIdPar - ид. нового параметра, oRes - 0 - ошибка, 1 - ок ----
-------- эфеекты: проверяем, что заданный тип параметра является типом ------
-------- параметра численного типа и указанный ЕИ существует, либо ----------
-------- заданный тип параметра является существует, в противном случае -----
-------- не добавляем запись ------------------------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION INS_PARAMETR ( 
	pShName VARCHAR(15), 
	pName VARCHAR(100), 
	pIdEI INTEGER, 
	pTypePar INTEGER
)
RETURNS TABLE (
	oIdPar INTEGER, 
	oRes INTEGER
) AS $$
DECLARE 
    vYes INTEGER;
    vNoTerm INTEGER;
	vYesNEn INTEGER;
    vYesEI INTEGER;
    vNN INTEGER := 22;   -- численное перечисление
    vReal INTEGER := 23; -- real
    vInt INTEGER := 24;  -- int
BEGIN
    oRes := 0;
    vYes := 0;
    vNoTerm := 0;
    vYesEI := 0;
    
	-- проверяем есть ли указанная ЕИ
    SELECT COUNT(*) INTO vYesEI 
	FROM EI WHERE ID_EI = pIdEI;
	
	SELECT oYes INTO vYesNEn 
	FROM IN_GR(vNN, pTypePar); -- параметр-перечисление
	
    SELECT COUNT(*) INTO vYes FROM PARAMETR1 
	WHERE SHORT_NAME = pShName OR NAME = pName; -- есть дубль имени
	
    SELECT COUNT(*) INTO vNoTerm 
	FROM CHEM_CLASS WHERE MAIN_CLASS = pTypePar; -- класс терминальный
    
    IF (vYes = 0 AND vNoTerm = 0) THEN
        BEGIN
            IF ((pTypePar = vReal OR pTypePar = vInt) AND vYesEI > 0) THEN
                oRes := 1; -- параметр численного типа или численного перечисления
            END IF;
            
            IF (oRes = 0 AND pTypePar <> vReal AND pTypePar <> vInt) THEN
                oRes := 1; -- параметр нечисленного типа или нечисленного перечисления
            END IF;
            
            IF (oRes = 1) THEN
                oIdPar := GEN_ID_PAR();
                INSERT INTO PARAMETR1 (ID_PAR, SHORT_NAME, NAME, EI_PAR, TYPE_P)
                VALUES (oIdPar, pShName, pName, pIdEI, pTypePar);
            END IF;
        END;
	END IF;
    RETURN QUERY SELECT oIdPar, oRes;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Добавляет новый параметр pIdPar для класса pIdClass -------
-------- вход: pIdPar - ид. параметра, pIdClass - ид.класса -----------------
-------- pMinVal - доп. мин. значение, pMaxVal -  доп. макс. значение -------
-------- выход: oNum - порядковый номер параметра в списке класса -----------
-------- oRes - 0- операция не выполнена, 1 - операция выполнена успешно ----
-------- эффекты: добавляет или обновляет записи в таблице PAR_CLASS1 -------
-------- в зависимости от наличия записи с указанными параметрами. ----------
-------- Если запись не существует, она добавляется с новым значением NUM. --
-------- Если запись уже существует, обновляются значения MIN_VAL и MAX_VAL.-
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION ADD_PARAMETR_CLASS ( 
	pIdPar INTEGER, 
	pIdClass INTEGER, 
	pMinVal DOUBLE PRECISION, 
	pMaxVal DOUBLE PRECISION
)
RETURNS TABLE (
	oNum INTEGER, 
	oRes INTEGER, 
	oIdClass INTEGER
) AS $$
DECLARE 
    vYes INTEGER;
    vIdClass INTEGER;
BEGIN
    oRes := 0;
    FOR vIdClass IN SELECT oIdGr FROM FIND_GR_GR(pIdClass) LOOP
        BEGIN
            SELECT COUNT(*) INTO vYes FROM PAR_CLASS1 
			WHERE ID_PAR = pIdPar AND ID_CLASS = vIdClass;
            IF (vYes = 0) THEN
                BEGIN
                    SELECT MAX(NUM) INTO oNum FROM PAR_CLASS1 
					WHERE ID_CLASS = vIdClass;
                    
					IF (oNum IS NULL) 
					THEN oNum := 0;
					END IF;
					
                    oNum := oNum + 1;
                    oRes := 1;
					
                    INSERT INTO PAR_CLASS1 (ID_PAR, ID_CLASS, NUM, MAX_VAL, MIN_VAL)
                    VALUES (pIdPar, vIdClass, oNum, pMaxVal, pMinVal); 
                    
					oIdClass := vIdClass; 
                END;
            ELSE
                BEGIN
                    UPDATE PAR_CLASS1
                    SET MAX_VAL = pMaxVal,
                        MIN_VAL = pMinVal
                    WHERE ID_CLASS = vIdClass AND ID_PAR = pIdPar;
                    oRes := 1;  
                END;
            END IF;
            RETURN NEXT;
        END;
	END LOOP;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------------------------- Вспомогательная функция --------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION FIND_GR_GR(pidGr INTEGER)
RETURNS TABLE (
    oIdGr INTEGER,
    oNameGr VARCHAR(100),
    oShortNameGr VARCHAR(15),
    oTerm INTEGER
) AS $$
DECLARE 
    vCountGr INTEGER;
    vIdGr INTEGER;
BEGIN
    vCountGr := 0; -- начальная инициализация
    oTerm := 0;
    oIdGr := 0;

    -- является ли исходная группа терминальной
    SELECT COUNT(*) INTO vCountGr FROM CHEM_CLASS WHERE MAIN_CLASS = pIdGr;

    IF (vCountGr = 0) THEN -- терминальный элемент
    	BEGIN
        	oIdGr := pIdGr;
        	oTerm := 1; 
        
        	SELECT NAME, SHORT_NAME INTO oNameGr, oShortNameGr 
			FROM CHEM_CLASS WHERE ID_CLASS = oIdGr;
        	RETURN NEXT;
    	END;
    ELSE -- найти входящие группы
    	BEGIN
        	oTerm := 0;
        	oIdGr := pIdGr;
        
        	SELECT NAME, SHORT_NAME INTO oNameGr, oShortNameGr FROM CHEM_CLASS 
			WHERE ID_CLASS = pIdGr;
        	RETURN NEXT;

        	FOR vIdGr IN (SELECT ID_CLASS FROM CHEM_CLASS WHERE MAIN_CLASS = pIdGr) LOOP
            	FOR oNameGr, oShortNameGr, oTerm IN 
                	SELECT f.oNameGr, f.oShortNameGr, f.oTerm FROM FIND_GR_GR(vIdGr) f LOOP
            	BEGIN
					oIdGr := vIdGr;
                	RETURN NEXT;
            	END;
				END LOOP;
			END LOOP;
    	END;
	END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Находит вектор значений параметров экземпляра продукта ----
-------- вход: pIdClass - ид. класса ----------------------------------------
-------- выход: таблица с найденными записями -------------------------------
-------- эффекты: извлекает данные из таблицы PAR_CLASS1 по заданному -------
-------- pIdClass, затем для каждой записи извлекает дополнительную ---------
-------- информацию о параметре и его единицах измерения из таблиц ----------
-------- PARAMETR1 и EI соответственно. -------------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION FIND_PAR_CLASS ( 
	pIdClass INTEGER
)
RETURNS TABLE (
	oIdPar INTEGER, 
	oShName VARCHAR(15), 
	oName VARCHAR(100), 
	oMinVal DOUBLE PRECISION,
    oMaxVal DOUBLE PRECISION, 
	oIdEi INTEGER, 
	oShNameEI VARCHAR(15)
) AS $$
BEGIN
    FOR oIdPar, oMinVal, oMaxVal IN 
	SELECT ID_PAR, MIN_VAL, MAX_VAL 
	FROM PAR_CLASS1 WHERE ID_CLASS = pIdClass LOOP
        BEGIN
            SELECT SHORT_NAME, NAME, EI_PAR INTO oShName, oName, oIdEI 
			FROM PARAMETR1 WHERE ID_PAR = oIdPar;
            
			SELECT SHORT_NAME INTO oShNameEI 
			FROM EI WHERE ID_EI = oIdEI;
            
			RETURN NEXT; 
        END;
	END LOOP;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------- Функция: Проверяет допустимость значения параметра экземпляра ------
-------- продукта и записывает его значение ---------------------------------
-------- вход: pIdProd - ид. продукта, pIdPar - ид. параметра ---------------
-------- выход: oRes - 0 - ошибка, 1 - ок -----------------------------------
-------- эффекты: 1) Проверяет существование записи в таблице PAR_PROD1 -----
-------- для переданных значений pIdProd и pIdPar.---------------------------
-------- 2) Обновляет значение VAL_R в таблице PAR_PROD1, если переданное ---
-------- значение pVal соответствует диапазону значений или является NULL. --
-------- 3) Устанавливает значение результата операции (oRes) в 1 в случае --
-------- успешного обновления и в 0 в противном случае. ---------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION WRITE_PAR ( 
	pIdProd INTEGER, 
	pIdPar INTEGER, 
	pVal DOUBLE PRECISION, 
	pStr VARCHAR(50),
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vYesPar INTEGER;
    vIdClass INTEGER;
    vMinVal DOUBLE PRECISION;
    vMaxVal DOUBLE PRECISION;
BEGIN
    SELECT COUNT(*) INTO vYesPar 
	FROM PAR_PROD1 WHERE ID_PAR = pIdPar AND PID_ROD = pIdProd;
    oRes := 1;
    
    IF (vYesPar > 0) THEN
    BEGIN
        SELECT ID_CLASS INTO vIdClass 
		FROM PROD WHERE ID_PROD = pIdProd;
        
        SELECT MIN_VAL, MAX_VAL INTO vMinVal, vMaxVal 
		FROM PAR_CLASS1 WHERE ID_PAR = pIdPar AND ID_CLASS = vIdClass;
        
        IF ((pVal >= vMinVal AND pVal <= vMaxVal) OR pVal IS NULL) THEN
        BEGIN
            oRes := 1;
            UPDATE PAR_PROD1 SET VAL_R = pVal 
			WHERE ID_PROD = pIdProd AND ID_PAR = pIdPar;
        END;
		END IF;
    END;
    ELSE
        oRes := 0;
	END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------- Функция: Находит тип параметра, проверяет допустимость -------------
-------- численного значения параметра экземпляра продукта и записывает -----
-------- его значение, проверяет наличие перечисления и его значения --------
-------- вход: pIdProd - ид. продукта, pIdPar - ид. параметра ---------------
-------- выход: oRes - 0 - ошибка, 1 - ок -----------------------------------
-------- эффекиы: 1) Функция проверяет наличие записи в таблице PAR_PROD1, --
-------- соответствующей переданным значениям pIdProd и pIdPar. -------------
-------- 2) Получение информации о параметре, включая его тип (vTypePar), ---
-------- единицы измерения (vEI), а также проверка возможности --------------
-------- использования числового или строкового значения параметра. ---------
-------- 3) Если параметр предполагает числовое значение (vYesR > 0), -------
-------- функция проверяет, соответствует ли переданное значение диапазону --
-------- (между vMinVal и vMaxVal), и обновляет значение VAL_R в таблице ----
-------- PAR_PROD1.----------------------------------------------------------
-------- 4) Если параметр предполагает строковое или перечислимое значение, -
-------- идентификатор перечисления pEnumVal и строковое значение pValStr ---
-------- обновляются в таблице PAR_PROD1. -----------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION WRITE_PAR_PROD ( 
	pIdProd INTEGER, 
	pIdPar INTEGER, 
	pVal DOUBLE PRECISION, 
	pValStr VARCHAR(150), 
	pEnumVal INTEGER,
	pNote VARCHAR(200),
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vYesPar INTEGER;
    vIdClass INTEGER;
    vYesR INTEGER;
    vYesEn INTEGER;
    vYesStr INTEGER;
    vInt INTEGER;
    vReal INTEGER;
    vStr INTEGER;
    vEI INTEGER;
    vTypePar INTEGER;
    vMinVal DOUBLE PRECISION;
    vMaxVal DOUBLE PRECISION;
BEGIN
    vInt := 24;
    vReal := 23;
    vStr := 20;
    vEI := 0;
    
    SELECT ID_CLASS INTO vIdClass 
	FROM PROD WHERE ID_PROD = pIdProd;
    
    SELECT COUNT(*) INTO vYesPar FROM PAR_PROD1 
	WHERE ID_PAR = pIdPar AND ID_PROD = pIdProd;
    
    IF (vYesPar > 0) THEN
    BEGIN
        SELECT EI_PAR, TYPE_P INTO vEI, vTypePar 
		FROM PARAMETR1 WHERE ID_PAR = pIdPar;
        
        SELECT oYes INTO vYesR 
		FROM IN_GR(vReal, vTypePar);
        
        SELECT COUNT(*) INTO vYesEn FROM POS_ENUM 
		WHERE ID_POS = pEnumVal AND ID_ENUM = vTypePar;
        
        SELECT oYes INTO vYesStr 
		FROM IN_GR(vStr, vTypePar);
        
		IF (pNote IS NOT NULL) THEN 
			UPDATE PAR_PROD1 SET NOTE = pNote;
		END IF;
		
        IF (vYesR > 0) THEN
        	BEGIN
            	SELECT MIN_VAL, MAX_VAL INTO vMinVal, vMaxVal FROM PAR_CLASS1 
				WHERE ID_PAR = pIdPar AND ID_CLASS = vIdClass;
			
            	IF (pVal >= vMinVal AND pVal <= vMaxVal) THEN
            		BEGIN
                		oRes := 1;
                		UPDATE PAR_PROD1 SET VAL_R = pVal 
						WHERE ID_PROD = pIdProd AND ID_PAR = pIdPar;
            		END;
				END IF;
        	END;
        ELSE
        	BEGIN
            	IF (vYesStr > 0 OR vYesEn > 0) THEN
            		BEGIN
                		oRes := 1;
                		UPDATE PAR_PROD1 SET ENUM_VAL = pEnumVal, VAL_STR = pValStr 
						WHERE ID_PROD = pIdProd AND ID_PAR = pIdPar;
            		END;
				END IF;
        	END;
		END IF;
    END;
    ELSE
        oRes := 0;
	END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------------------------
-------------------------- Вспомогательная функция --------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION IN_GR(
    pMainGr INTEGER,
    pGr INTEGER
)
RETURNS TABLE (
    oYes INTEGER
) AS $$
BEGIN
    oYes := 0;

    SELECT COUNT(*) INTO oYes 
    FROM FIND_GR_GR(pMainGr)
    WHERE oIdGr = pGr;

    RETURN QUERY SELECT oYes;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------- Функция: Добавление параметра продукта в таблицу PAR_PROD1 ---------
-------- вход: pIdProd, pIdpar, pValInt, pValR, pValStr, pEnVal, pNote ------
-------- выход: oRes - 0 - ошибка, 1 - ок -----------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION ADD_PAR_PROD (
    pIdProd INTEGER,           -- Идентификатор продукта
    pIdPar  INTEGER,           -- Идентификатор параметра
    pValInt INTEGER,           -- Целочисленное значение параметра
	pValR   DOUBLE PRECISION,  -- Рациональнное значение параметра
    pValStr VARCHAR(150),      -- Текстовое представление параметра
    pEnVal  INTEGER,           -- Идентификатор перечисления (если есть)
	pNote    VARCHAR(200),     -- Примечание к параметру
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE 
    vProdCount INTEGER;
    vParCount INTEGER;
    vEnCount INTEGER;
    vMinVal DOUBLE PRECISION;
    vMaxVal DOUBLE PRECISION;
    vIdClass INTEGER;
	vInClassR BOOLEAN := false;
	vInClassI BOOLEAN := false;
BEGIN
	oRes := 0;

    -- Проверка существования продукта
    SELECT count(*) INTO vProdCount FROM PROD WHERE ID_PROD = pIdProd;
    IF (vProdCount = 0) THEN
        RAISE EXCEPTION 'Не существует продукта';
    END IF;

    -- Проверка существования параметра
    SELECT count(*) INTO vParCount FROM PARAMETR1 WHERE ID_PAR = pIdPar;
    IF (vParCount = 0) THEN
        RAISE EXCEPTION 'Не существует параметра';
    END IF;

    -- Проверка существования перечисления, если указано
    IF (pEnVal IS NOT NULL) THEN
        SELECT count(*) INTO vEnCount FROM POS_ENUM WHERE ID_POS = pEnVal;
        IF (vEnCount = 0) THEN
            RAISE EXCEPTION 'Не существует перечисления';
        END IF;
    END IF;

    -- Получение класса продукта
    SELECT ID_CLASS INTO vIdClass FROM PROD WHERE ID_PROD = pIdProd;

    -- Получение минимального и максимального значения параметра для данного класса
    SELECT MIN_VAL, MAX_VAL INTO vMinVal, vMaxVal 
	FROM PAR_CLASS1 WHERE ID_PAR = pIdPar AND ID_CLASS = vIdClass;

    -- Проверка значения параметра на соответствие диапазону
    IF (pValInt IS NULL AND pValR IS NULL OR 
		(pValInt >= vMinVal AND pValInt <= vMaxVal) OR
	   	(pValR >= vMinVal AND pValR <= vMaxVal)) THEN
        IF (pValInt IS NOT NULL) THEN
			vInClassI := true;
		END IF;
		
		IF (pValR IS NOT NULL) THEN
			vInClassR := true;
		END IF;
		
		IF (vInClassI) THEN
        	INSERT INTO PAR_PROD1 (ID_PROD, ID_PAR, VAL_INT, VAL_STR, ENUM_VAL, NOTE)
        	VALUES (pIdProd, pIdPar, pValInt, pValStr, pEnVal, pNote);
			
			IF NOT FOUND THEN
        		oRes := 0;
    		ELSE
        		oRes := 1;
    		END IF;
		END IF;
		
		IF (vInClassR) THEN
			INSERT INTO PAR_PROD1 (ID_PROD, ID_PAR, VAL_R, VAL_STR, ENUM_VAL, NOTE)
        	VALUES (pIdProd, pIdPar, pValR, pValStr,  pEnVal, pNote);
			
			IF NOT FOUND THEN
        		oRes := 0;
    		ELSE
        		oRes := 1;
    		END IF;
		END IF;
		
		IF (NOT vInClassI AND NOT vInClassR) THEN
        	INSERT INTO PAR_PROD1 (ID_PROD, ID_PAR, VAL_STR, ENUM_VAL, NOTE)
        	VALUES (pIdProd, pIdPar, pValStr, pEnVal, pNote);
			
			IF NOT FOUND THEN
        		oRes := 0;
    		ELSE
        		oRes := 1;
    		END IF;
		END IF;
    ELSE
        RAISE EXCEPTION 'Не соответствует области значений';
    END IF;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------- Функция: Находит вектор значений параметров экземпляра продукта ----
-------- вход: pIdProd - ид. изделия ----------------------------------------
-------- выход: oIdPar - ид. параметра, oShName - обозначение параметра, ----
-------- oName - имя параметра, oValR - значения вещ параметра --------------
-------- oValStr – значение строкового параметра, oNamePic- имя изобраения --
-------- oBlob – изображение поз. -------------------------------------------
-------- эффекты: функция извлекает информацию о параметрах -----------------
-------- продукта с идентификатором pIdProd и их значения из таблицы --------
-------- PAR_PROD1. Для каждого параметра определяется тип, проверяется -----
-------- наличие изображения и, если оно есть, извлекается из таблицы -------
-------- POS_ENUM. Функция возвращает набор данных, включая информацию ------
-------- о параметрах и их значения, а также, при наличии, изображение и ----
-------- его имя из перечисления. -------------------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION FIND_PAR_PROD (
	pIdProd INTEGER,
	pIdClass INTEGER
)
RETURNS TABLE (
	oIdPar INTEGER, 
	oShName VARCHAR(15), 
	oName VARCHAR(100), 
	oValR DOUBLE PRECISION, 
	oValInt INTEGER, 
	oShNameEI VARCHAR(15), 
	oValStr VARCHAR(50), 
	oEnumVal INTEGER, 
	oNameValEn VARCHAR(20), 
	oBlob BYTEA
) AS $$
DECLARE 
    vIdClass INTEGER;
    vTypePar INTEGER;
    vYesPic INTEGER;
    vEnPic INTEGER;
BEGIN
    vEnPic := 21;
    FOR oIdPar, oShName, oName, oShNameEI IN 
	SELECT f.oIdPar, f.oShName, f.oName, f.oShNameEI FROM FIND_PAR_CLASS(pIdClass) f LOOP
    BEGIN
        SELECT VAL_R, VAL_STR, VAL_INT, ENUM_VAL 
		INTO oValR, oValStr, oValInt, oEnumVal 
		FROM PAR_PROD1 WHERE ID_PROD = pIdProd AND ID_PAR = oIdPar;
        
        SELECT TYPE_P INTO vTypePar 
		FROM PARAMETR1 WHERE ID_PAR = oIdPar;
        
        oNameValEn := 0;
        SELECT oYes INTO vYesPic FROM IN_GR(vEnPic, vTypePar);
        
        IF (oEnumVal IS NOT NULL) THEN
            SELECT PIC_VAL, E_NAME INTO oBlob, oNameValEn 
			FROM POS_ENUM WHERE ID_POS = oEnumVal;
		END IF;
        
        IF (vYesPic = 0) THEN 
            oBlob := NULL;
		END IF;
        
        RETURN NEXT;
    END;
	END LOOP;
END;
$$ LANGUAGE plpgsql;



-----------------------------------------------------------------------------
-------- Функция: Добавление нового значения в таблицу POS_ENUM -------------
-------- вход: pid_enum - ид. перечисления, pshort_name - обозначение -------
-------- pname - имя, prval - вещественное значение, pint_val – -------------
-------- целочисленное значение, ppic_val – путь к файлу рисунка ------------
-----------------------------------------------------------------------------
--(не уверена насчет проверик и вставляемых значений)
CREATE OR REPLACE FUNCTION INS_VAL_ENUM (
	pIdEnum INTEGER,
    pShortName VARCHAR(15),
    pName VARCHAR(50),
    pRval DOUBLE PRECISION,
    pIntVal INTEGER,
    pPicVal VARCHAR(150),
	OUT oIdPosENUM INTEGER, -- id новой записи перечисления
    OUT oRes INTEGER -- код возврата: 1 - успешно, 0 - ошибка вставки
)
LANGUAGE plpgsql
AS $$
DECLARE
    vIdPos INTEGER;
    vNum INTEGER;
    vIdEnumCount INTEGER;
BEGIN
    oRes := 1;
    oIdPosENUM := GEN_POS_ENUM();
	
	SELECT COUNT(*) FROM CHEM_CLASS WHERE ID_CLASS = pIdEnum INTO vIdEnumCount;
    IF vIdEnumCount = 0 THEN
		oRes := 0;
        oIdPosENUM := NULL;
        RAISE EXCEPTION 'ENUM does not exist';
    END IF;
    
    SELECT COUNT(*) + 1 FROM POS_ENUM WHERE ID_ENUM = pIdEnum INTO vNum;
	
    INSERT INTO POS_ENUM (ID_POS, ID_ENUM, NUM, SHORT_NAME, E_NAME, RVAL, INT_VAL, PIC_VAL)
    VALUES (oIdPosENUM, pIdEnum, vNum, pShortName, pName, pRval, pIntVal, pPicVal);

    IF NOT FOUND THEN
        oRes := 0;
        oIdPosENUM := NULL;
    END IF;
END;
$$;

-----------------------------------------------------------------------------
-------- Функция: Удаление перечисления POS_ENUM ----------------------------
-------- вход: pIdEnum - ид. перечисления -----------------------------------
-------- выход: oRes - 0 - ошибка, 1 - ок -----------------------------------
-------- эффекты: Если есть параметр типа удаляемого перечисления, ----------
-------- то удаление не происходит ------------------------------------------
-----------------------------------------------------------------------------
--(посмотреть эффекты)
CREATE OR REPLACE FUNCTION DEL_ENUM (
	pIdEnum INTEGER,
	OUT oRes INTEGER -- код возврата: 1 - успешно, 0 - ошибка вставки
)
RETURNS INTEGER AS $$
BEGIN
	oRes := 1;
	DELETE FROM POS_ENUM WHERE ID_ENUM = pIdEnum;
    
	IF NOT FOUND THEN
        oRes := 0;
    END IF;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------- Функция: Удаление значения перечисления POS_ENUM -------------------
-------- вход: pIdPos - ид. значения ----------------------------------------
-------- выход: oRes - 0 - ошибка, 1 - ок -----------------------------------
-------- эффекты: Если есть параметр типа удаляемого перечисления, ----------
-------- то удаление не происходит ------------------------------------------
-----------------------------------------------------------------------------
--(посмотреть эффекты)
CREATE OR REPLACE FUNCTION DEL_ENUM_VAL (
	pIdPos INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER AS $$
DECLARE
    vIdEnum INTEGER;
BEGIN
	oRes := 1;
	
    SELECT ID_ENUM FROM POS_ENUM WHERE ID_POS = pIdPos INTO vIdEnum;

    DELETE FROM POS_ENUM WHERE ID_ENUM = vIdEnum AND ID_POS = pIdPos;
     
	IF NOT FOUND THEN
        oRes := 0;
    END IF;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
-------- Функция: Переместить значение вверх --------------------------------
-------- вход: pIdPos - ид. значения ----------------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE UP_VAL (
    pIdPos INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    vMinNum INTEGER;
    vIdEnum INTEGER;
    vIdPred INTEGER;
    vIdPosCount INTEGER;
    vCurNum INTEGER;
BEGIN
    SELECT COUNT(*) FROM POS_ENUM WHERE ID_POS = pIdPos INTO vIdPosCount;
    IF vIdPosCount = 0 THEN
        RAISE EXCEPTION 'Не существует элемента перечисления';
    END IF;

    SELECT ID_ENUM, NUM FROM POS_ENUM 
	WHERE ID_POS = pIdPos INTO vIdEnum, vCurNum;
    
    SELECT MIN(NUM) FROM POS_ENUM 
	WHERE ID_ENUM = vIdEnum INTO vMinNum;

    IF vCurNum > vMinNum THEN
        SELECT ID_POS FROM POS_ENUM 
		WHERE ID_ENUM = vIdEnum AND NUM = (vCurNum - 1) INTO vIdPred;
        
        UPDATE POS_ENUM SET NUM = 0 WHERE ID_POS = pIdPos;
        UPDATE POS_ENUM SET NUM = vCurNum WHERE ID_POS = vIdPred;
        UPDATE POS_ENUM SET NUM = (vCurNum - 1) WHERE ID_POS = pIdPos;
    ELSE
        RAISE EXCEPTION 'Элемент уже первый в списке';
    END IF;
END;
$$;

-----------------------------------------------------------------------------
-------- Функция: Переместить значение вниз ---------------------------------
-------- вход: pIdPos - ид. значения ----------------------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE PROCEDURE DOWN_VAL (
    pIdPos INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    vMaxNum INTEGER;
    vIdEnum INTEGER;
    vIdNext INTEGER;
    vIdPosCount INTEGER;
    vCurNum INTEGER;
BEGIN
    SELECT COUNT(*) FROM POS_ENUM 
	WHERE ID_POS = pIdPos INTO vIdPosCount;
    IF vIdPosCount = 0 THEN
        RAISE EXCEPTION 'Не существует элемента перечисления';
    END IF;

    SELECT ID_ENUM, NUM FROM POS_ENUM 
	WHERE ID_POS = pIdPos INTO vIdEnum, vCurNum;
    
    SELECT MAX(NUM) FROM POS_ENUM 
	WHERE ID_ENUM = vIdEnum INTO vMaxNum;

    IF vCurNum < vMaxNum THEN
        SELECT ID_POS FROM POS_ENUM 
		WHERE ID_ENUM = vIdEnum AND NUM = (vCurNum + 1) INTO vIdNext;
        
        UPDATE POS_ENUM SET NUM = 0 WHERE ID_POS = pIdPos;
        UPDATE POS_ENUM SET NUM = vCurNum WHERE ID_POS = vIdNext;
        UPDATE POS_ENUM SET NUM = (vCurNum + 1) WHERE ID_POS = pIdPos;
    ELSE
        RAISE EXCEPTION 'Элемент уже последний в списке';
    END IF;
END;
$$;

-----------------------------------------------------------------------------
-------- Функция: Находит список значений перечисления ----------------------
-------- вход: pid_enum - ид. перечисления ----------------------------------
-------- выход: result_table - таблица значений -----------------------------
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION FIND_LIST_ENUM (
    pIdEnum INTEGER
)
RETURNS TABLE (
    oIdPos INTEGER,
    oNum INTEGER,
    oShortName VARCHAR(15),
    oName VARCHAR(50),
    oRval DOUBLE PRECISION,
    oIntVal INTEGER,
    oPicVal VARCHAR(150)
)
AS $$
BEGIN
    RETURN QUERY
    SELECT ID_POS, NUM, SHORT_NAME, E_NAME, RVAL, INT_VAL, PIC_VAL
    FROM POS_ENUM
    WHERE ID_ENUM = pIdEnum
    ORDER BY NUM;
END;
$$ LANGUAGE plpgsql;


-----------------------------------------------------------------------------
----------- функция: Добавление новой единицы измерения-----------
----возвращает запись со значениями нового идентификатора ЕИ и кода результата опрерации
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION INS_EI(
    pCode VARCHAR(15),
    pShortName VARCHAR(10),
    pName VARCHAR(50),
    OUT oIdEI INTEGER, -- id новой единицы измерения
    OUT oRes INTEGER -- код возврата: 1 - успешно, 0 - ошибка вставки
)
RETURNS RECORD
LANGUAGE plpgsql
AS $$
BEGIN
    oRes := 1; -- Изначально считаем, что операция успешна

    -- Получение нового идентификатора из последовательности
     oIdEI := GEN_ID_EI();

    -- Вставка новой записи в таблицу EI
    INSERT INTO EI(ID_EI, SHORT_NAME, NAME, CODE)
    VALUES (oIdEI, pShortName, pName, pCode);

    -- Если произошла ошибка при вставке, то обновим oRes и не вернем oIdEI
    IF NOT FOUND THEN
        oRes := 0;
        oIdEI := NULL;
    END IF;

    RETURN;
END;
$$;

-----------------------------------------------------------------------------
----------- функция: Удаление ед. измерения -----------
----------- возвращает код результата операции:
----------- 1 - успешно, 0 - ошибка вставки
-----------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION DEL_EI(
	pIdEI INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER
LANGUAGE plpgsql
AS $$
DECLARE
  	vCount INTEGER;
BEGIN
	oRes := 1;
	-- Проверяем, есть ли ссылки на ЕИ в других таблицах
  	SELECT COUNT(*) INTO vCount FROM CHEM_CLASS  
	WHERE BASE_EI = pIdEI;
  
  	IF vCount > 0 THEN
    	-- Если есть ссылки, то удаление невозможно
    	oRes := 0;
  	ELSE
    	-- Если ссылок нет, то удаляем ЕИ
    	DELETE FROM EI 
		WHERE ID_EI = pIdEI;
    	
		IF NOT FOUND THEN
        	oRes := 0;
    	END IF;
  	END IF;
END;
$$;

-----------------------------------------------------------------
----------- функция: Добавление нового класса изделий -----------
CREATE OR REPLACE FUNCTION ADD_CHEM_CLASS(
    pShortName VARCHAR(50),
    pName VARCHAR(200),
   	pBaseEI INTEGER,
    pMainClass INTEGER,
	OUT oIdEI INTEGER, -- id нового класса изделия
    OUT oRes INTEGER -- код возврата
)
RETURNS RECORD
LANGUAGE plpgsql
AS $$
DECLARE
    vEiCount INTEGER;
BEGIN
	oRes := 0;
	oIdEI := NULL;
	
    SELECT COUNT(*) INTO vEiCount FROM EI 
	WHERE ID_EI = pBaseEI;
    
	IF vEiCount != 0 THEN
        oIdEI := GEN_ID_CLASS();

        INSERT INTO CHEM_CLASS (ID_CLASS, SHORT_NAME, NAME, BASE_EI, MAIN_CLASS)
        VALUES (oIdEI, pShortName, pName, pBaseEI, pMainClass);
		
		IF NOT FOUND THEN
        	oRes := 0;
			oIdEI := NULL;
    	ELSE 
			oRes := 1;
		END IF;
    END IF;
END;
$$;

---------------------------------------------------------------
----------- функция: Удаление класса изделий -----------
CREATE OR REPLACE FUNCTION DELETE_CHEM_CLASS(
    pIdClass INTEGER,
	OUT oRes INTEGER
)
RETURNS INTEGER
LANGUAGE plpgsql
AS $$
DECLARE
  	vCount INTEGER;
BEGIN
	oRes := 1;
	
	-- Проверяем, есть ли ссылки в других таблицах
  	SELECT COUNT(*) INTO vCount FROM CHEM_CLASS 
	WHERE MAIN_CLASS = pIdClass;
  
  	IF vCount > 0 THEN
    	-- Если есть ссылки, то удаление невозможно
    	oRes := 0;
  	ELSE
    	-- Если ссылок нет, то удаляем
    	DELETE FROM CHEM_CLASS
    	WHERE ID_CLASS = pIdClass;
    	
		IF NOT FOUND THEN
        	oRes := 0;
		END IF;
  	END IF;
END;
$$;

-------------------------------------------------------------------
----------- функция: Изменение родителя у класса изделия -----------
CREATE OR REPLACE FUNCTION CHANGE_PARENT_CHEM_CLASS(
    pIdClass INTEGER,
    pNewMainClass INTEGER,
    OUT oRes INTEGER
)
RETURNS INTEGER
LANGUAGE plpgsql
AS $$
DECLARE
    vParentCount INTEGER;
    vIsCyclicDependency BOOLEAN;
BEGIN
    oRes := 0;

	-- Проверка существования нового родительского класса
    SELECT COUNT(*) INTO vParentCount FROM CHEM_CLASS
    WHERE ID_CLASS = pNewMainClass;
	
	IF vParentCount = 0 THEN
		RETURN;
	END IF;
	
    -- Проверка на циклическую зависимость
    SELECT EXISTS(
        WITH RECURSIVE class_tree AS (
            SELECT ID_CLASS, MAIN_CLASS
            FROM CHEM_CLASS
            WHERE ID_CLASS = pNewMainClass
            UNION ALL
            SELECT c.ID_CLASS, c.MAIN_CLASS
            FROM CHEM_CLASS c
            JOIN class_tree ct ON c.MAIN_CLASS = ct.ID_CLASS
        )
        SELECT 1
        FROM class_tree
        WHERE ID_CLASS = pIdClass
    ) INTO vIsCyclicDependency;

    -- Если есть циклическая зависимость, выходим без изменений
    IF vIsCyclicDependency THEN
        RETURN;
    END IF;

    UPDATE CHEM_CLASS
    SET MAIN_CLASS = pNewMainClass
    WHERE ID_CLASS = pIdClass;

    IF NOT FOUND THEN
        oRes := 0;
    ELSE
        oRes := 1;
    END IF;
END;
$$;

--------------------------------------------------------------------------
----------- функция: Добавление нового продукта в таблицу prod -----------
CREATE OR REPLACE FUNCTION ADD_PROD(
    pShortName VARCHAR(50),
    pName VARCHAR(250),
    pIdClass INTEGER,
	OUT oIdEI INTEGER,
	OUT oRes INTEGER
)
RETURNS RECORD
LANGUAGE plpgsql
AS $$
DECLARE
	par RECORD;
    vClassCount INTEGER;
BEGIN
	oRes := 0;
	oIdEI := NULL;
	
    SELECT COUNT(*) INTO vClassCount FROM CHEM_CLASS 
	WHERE ID_CLASS = pIdClass;
    
	IF vClassCount != 0 THEN
        oIdEI := GEN_ID_PROD();

        INSERT INTO prod (ID_PROD, SHORT_NAME, NAME, ID_CLASS)
        VALUES (oIdEI, pShortName, pName, pIdClass);
		
		IF NOT FOUND THEN
        	oRes := 0;
			oIdEI := NULL;
		ELSE
			oRes := 1;
			
			FOR par IN SELECT ID_PAR FROM PARAMETR1 LOOP
        	PERFORM ADD_PAR_PROD(oIdEI, par.ID_PAR, NULL, NULL, NULL, NULL, NULL);
    END LOOP;
			
		END IF;
    END IF;
END;
$$;

-----------------------------------------------------------------
----------- функция: Удаления продукта из таблицы prod -----------
CREATE OR REPLACE FUNCTION DELETE_PROD(
    pIdProd INTEGER,
    OUT oRes INTEGER
)
RETURNS INTEGER
LANGUAGE plpgsql
AS $$
BEGIN
	DELETE FROM PROD
    WHERE ID_PROD = pIdProd;
	
    IF NOT FOUND THEN
        oRes := 0;
    ELSE
	oRes := 1;
    END IF;
END;
$$;

------------------------------------------------------------------
----------- функция: Поиск родителя у класса изделия -----------
CREATE OR REPLACE FUNCTION FIND_PARENT(
    pIdClass INTEGER
)
RETURNS TABLE (
    PARENT_ID INTEGER,
    PARENT_SHORT_NAME VARCHAR(50),
    PARENT_NAME VARCHAR(200),
    PARENT_BASE_EI INTEGER,
    PARENT_MAIN_CLASS INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_current_id INTEGER;
    v_parent_id INTEGER;
BEGIN
    v_current_id := pIdClass;

    -- Пока есть родительский элемент
    LOOP
        SELECT MAIN_CLASS INTO v_parent_id
        FROM CHEM_CLASS
        WHERE ID_CLASS = v_current_id;

        IF v_parent_id IS NULL THEN
            EXIT;
        END IF;

        SELECT SHORT_NAME, NAME, BASE_EI, MAIN_CLASS
        INTO PARENT_SHORT_NAME, PARENT_NAME, PARENT_BASE_EI, PARENT_MAIN_CLASS
        FROM CHEM_CLASS
        WHERE ID_CLASS = v_parent_id;

        PARENT_ID := v_parent_id;

        RETURN NEXT;
		EXIT;

        v_current_id := v_parent_id;
    END LOOP;
END;
$$;

-----------------------------------------------------------------
----------- функция: Поиск потомков у класса изделия -----------
CREATE OR REPLACE FUNCTION FIND_CHILD(
    pIdClass INTEGER
)
RETURNS TABLE (
    CHILD_ID INTEGER,
    CHILD_SHORT_NAME VARCHAR(50),
    CHILD_NAME VARCHAR(200),
    CHILD_BASE_EI INTEGER,
    CHILD_MAIN_CLASS INTEGER
)
LANGUAGE plpgsql
AS $$
DECLARE
    rec RECORD;
BEGIN
    FOR rec IN
        SELECT ID_CLASS, SHORT_NAME, NAME, BASE_EI, MAIN_CLASS
        FROM CHEM_CLASS
        WHERE MAIN_CLASS = pIdClass
    LOOP
        -- Добавляем найденную запись в результирующую таблицу
        CHILD_ID := rec.ID_CLASS;
        CHILD_SHORT_NAME := rec.SHORT_NAME;
        CHILD_NAME := rec.NAME;
        CHILD_BASE_EI := rec.BASE_EI;
        CHILD_MAIN_CLASS := rec.MAIN_CLASS;
        RETURN NEXT;

    END LOOP;

    -- Если не было найдено ни одной записи, возвращаем пустую таблицу
    IF NOT FOUND THEN
        RETURN;
    END IF;
END;
$$;

-----------------------------------------------
----------- функция: Поиск продукта -----------
CREATE OR REPLACE FUNCTION FIND_PROD(
    pIdProd INTEGER
)
RETURNS TABLE (
    ID_PROD INTEGER,
    SHORT_NAME VARCHAR(50),
    NAME VARCHAR(250),
    ID_CLASS INTEGER
)
LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
    SELECT p.ID_PROD, p.SHORT_NAME, p.NAME, p.ID_CLASS
    FROM PROD p
    WHERE p.ID_PROD = pIdProd;
END;
$$;