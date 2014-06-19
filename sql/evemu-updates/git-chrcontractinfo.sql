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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `chrcontractinfo`
--
