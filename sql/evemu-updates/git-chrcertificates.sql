--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `chrcertificates`
--

CREATE TABLE IF NOT EXISTS `chrcertificates` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `characterID` int(10) NOT NULL,
  `certificateID` int(10) NOT NULL,
  `grantDate` bigint(20) NOT NULL,
  `visibilityFlags` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `chrcertificates`
--

