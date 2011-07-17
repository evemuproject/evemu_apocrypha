-- phpMyAdmin SQL Dump
-- version 3.3.9
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 04-04-2011 a las 20:28:20
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
  `numOutstandingContractsNonCorp` int(11) NOT NULL,
  `numOutstandingContractsForCorp` int(11) NOT NULL,
  `numOutstandingContracts` int(11) NOT NULL,
  `numContractsLeft` int(11) NOT NULL,
  `numRequiresAttention` int(11) NOT NULL,
  `numRequiresAttentionCorp` int(11) NOT NULL,
  `numAssignedTo` int(11) NOT NULL,
  `numAssignedToCorp` int(11) NOT NULL,
  `numBiddingOn` int(11) NOT NULL,
  `numInProgress` int(11) NOT NULL,
  `numBiddingOnCorp` int(11) NOT NULL,
  `numInProgressCorp` int(11) NOT NULL,
  PRIMARY KEY (`characterID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

