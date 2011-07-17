-- phpMyAdmin SQL Dump
-- version 3.3.9
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 04-04-2011 a las 20:22:55
-- Versión del servidor: 5.5.8
-- Versión de PHP: 5.3.5

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

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
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Volcar la base de datos para la tabla `contract_bids`
--

