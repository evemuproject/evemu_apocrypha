--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `contract_bids`
--

CREATE TABLE IF NOT EXISTS `contract_bids` (
  `bidID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `contractID` int(10) unsigned NOT NULL,
  `issuerID` int(10) unsigned NOT NULL,
  `quantity` double unsigned NOT NULL,
  `issuerCorpID` int(10) unsigned NOT NULL,
  `issuerStationID` int(10) unsigned NOT NULL,
  `issuerSolarSystemID` int(10) unsigned NOT NULL,
  `issuerRegionID` int(10) unsigned NOT NULL,
  PRIMARY KEY (`bidID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `contract_bids`
--

