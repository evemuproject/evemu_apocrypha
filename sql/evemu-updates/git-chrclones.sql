--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `chrclones`
--

CREATE TABLE IF NOT EXISTS `chrclones` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `characterID` int(10) NOT NULL,
  `stationID` int(10) NOT NULL,
  `itemID` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `chrclones`
--

