--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `chrboosters`
--

CREATE TABLE IF NOT EXISTS `chrboosters` (
  `itemID` int(10) NOT NULL,
  `expiretime` bigint(20) NOT NULL,
  `plugDate` bigint(20) NOT NULL,
  `ownerID` int(11) NOT NULL,
  PRIMARY KEY (`itemID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `chrboosters`
--

