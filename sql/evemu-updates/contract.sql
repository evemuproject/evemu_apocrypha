-- phpMyAdmin SQL Dump
-- version 3.3.9
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 04-04-2011 a las 20:22:35
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
-- Estructura de tabla para la tabla `contract`
--

CREATE TABLE IF NOT EXISTS `contract` (
  `contractID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `issuerID` int(10) unsigned NOT NULL DEFAULT '0',
  `issuerCorpID` int(10) unsigned NOT NULL,
  `type` int(10) NOT NULL,
  `avail` int(10) NOT NULL,
  `assigneeID` int(10) NOT NULL,
  `expiretime` int(10) NOT NULL,
  `duration` int(10) NOT NULL,
  `startStationID` int(10) unsigned NOT NULL,
  `endStationID` int(10) unsigned NOT NULL,
  `startSolarSystemID` int(10) unsigned NOT NULL,
  `endSolarSystemID` int(10) unsigned DEFAULT NULL,
  `startRegionID` int(10) unsigned NOT NULL,
  `endRegionID` int(10) unsigned DEFAULT NULL,
  `price` int(10) NOT NULL,
  `reward` int(10) NOT NULL,
  `collateral` int(10) NOT NULL,
  `title` varchar(85) DEFAULT NULL,
  `description` text,
  `forCorp` tinyint(1) NOT NULL,
  `status` int(10) NOT NULL,
  `isAccepted` tinyint(1) NOT NULL,
  `acceptorID` int(10) DEFAULT NULL,
  `dateIssued` bigint(20) DEFAULT NULL,
  `dateExpired` bigint(20) DEFAULT NULL,
  `dateAccepted` bigint(20) DEFAULT NULL,
  `dateCompleted` bigint(20) DEFAULT NULL,
  `volume` int(10) unsigned DEFAULT NULL,
  `acceptorWalletKey` int(10) DEFAULT NULL,
  `issuerAllianceID` int(10) DEFAULT NULL,
  `issuerWalletKey` int(10) DEFAULT NULL,
  PRIMARY KEY (`contractID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=8 ;

--
-- Volcar la base de datos para la tabla `contract`
--

