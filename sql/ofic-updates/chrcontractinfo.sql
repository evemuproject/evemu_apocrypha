-- phpMyAdmin SQL Dump
-- version 3.3.9
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 26-05-2011 a las 20:50:58
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
-- Estructura de tabla para la tabla `chrcontractinfo`
--

CREATE TABLE IF NOT EXISTS `chrcontractinfo` (
  `characterID` int(10) NOT NULL,
  `numOutstandingContractsNonCorp` int(10) NOT NULL,
  `numOutstandingContractsForCorp` int(10) NOT NULL,
  `numOutstandingContracts` int(10) NOT NULL,
  `numContractsLeft` int(10) NOT NULL,
  `numRequiresAttention` int(10) NOT NULL,
  `numRequiresAttentionCorp` int(10) NOT NULL,
  `numAssignedTo` int(10) NOT NULL,
  `numAssignedToCorp` int(10) NOT NULL,
  `numBiddingOn` int(10) NOT NULL,
  `numInProgress` int(10) NOT NULL,
  `numBiddingOnCorp` int(10) NOT NULL,
  `numInProgressCorp` int(10) NOT NULL,
  PRIMARY KEY (`characterID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcar la base de datos para la tabla `chrcontractinfo`
--

ALTER TABLE `character_` ADD `lastLogin` BIGINT( 20 ) NULL DEFAULT NULL 

