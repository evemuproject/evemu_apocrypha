--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `chrshipinsurances`
--

CREATE TABLE IF NOT EXISTS `chrshipinsurances` (
  `insuranceID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ownerID` int(10) unsigned NOT NULL,
  `shipID` int(10) unsigned NOT NULL,
  `fraction` int(10) NOT NULL,
  `startDate` bigint(20) NOT NULL,
  `endDate` bigint(20) NOT NULL,
  PRIMARY KEY (`insuranceID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `chrshipinsurances`
--

