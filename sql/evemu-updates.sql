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
  `price` double NOT NULL,
  `reward` double NOT NULL,
  `collateral` double NOT NULL,
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
  `volume` double DEFAULT NULL,
  `requiresAttention` tinyint(1) NOT NULL DEFAULT '0',
  `crateID` int(10) unsigned NOT NULL DEFAULT '0',
  `issuerWalletKey` int(10) unsigned NOT NULL DEFAULT '0',
  `issuerAllianceID` int(10) unsigned NOT NULL DEFAULT '0',
  `acceptorWalletKey` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`contractID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `contract`
--

--
-- Base de datos: `evemu`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `contracts_items`
--

CREATE TABLE IF NOT EXISTS `contracts_items` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `contractID` int(10) NOT NULL,
  `itemTypeID` int(10) NOT NULL,
  `quantity` int(10) NOT NULL,
  `inCrate` tinyint(1) NOT NULL,
  `itemID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcar la base de datos para la tabla `contracts_items`
--

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

ALTER TABLE `corporation`
ADD `walletDivision2` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '2nd Wallet Division',
ADD `walletDivision3` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '3rd Wallet Division',
ADD `walletDivision4` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '4th Wallet Division',
ADD `walletDivision5` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '5th Wallet Division',
ADD `walletDivision6` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '6th Wallet Division',
ADD `walletDivision7` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '7th Wallet Division',
ADD `divisionBalance2` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance3` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance4` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance5` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance6` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance7` DOUBLE NOT NULL DEFAULT '0';

ALTER TABLE `corporationstatic`
ADD `walletDivision2` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '2nd Wallet Division',
ADD `walletDivision3` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '3rd Wallet Division',
ADD `walletDivision4` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '4th Wallet Division',
ADD `walletDivision5` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '5th Wallet Division',
ADD `walletDivision6` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '6th Wallet Division',
ADD `walletDivision7` VARCHAR( 100 ) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '7th Wallet Division',
ADD `divisionBalance2` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance3` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance4` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance5` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance6` DOUBLE NOT NULL DEFAULT '0',
ADD `divisionBalance7` DOUBLE NOT NULL DEFAULT '0';

UPDATE `corporation` SET creatorID=1 WHERE creatorID=0;
UPDATE `corporationstatic` SET creatorID=1 WHERE creatorID=0;
-- No longer needed
DROP TABLE entityStatic;
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014839, 500, 0, 50000000, 1000, 45, 54, 1000173, 30000380, 20000054, 10000004, 'Polaris I - Station', 35263733760, 4932526080, 55459307520, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014842, 500, 0, 50000000, 1000, 45, 56, 1000173, 30000380, 20000054, 10000004, 'Polaris II - Station', 65561272320, 9169920000, -95195504640, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014845, 500, 0, 50000000, 1000, 45, 1927, 1000173, 30000380, 20000054, 10000004, 'Polaris III - Station', 190772305920, 26684006400, -94657044480, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014936, 500, 0, 50000000, 1000, 45, 1932, 1000132, 30002854, 20000419, 10000034, 'Rat Annihilation and Termination Squad Station', -83125002240, -8509562880, -27283906560, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014848, 500, 0, 50000000, 1000, 45, 2499, 1000173, 30000380, 20000054, 10000004, 'Polaris IV - Station', 266567393280, 37291008000, 98144133120, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014867, 500, 0, 50000000, 1000, 45, 12242, 144719292, 30000895, 20000131, 10000010, 'IMK-K1 IX - Moon 1 - Manufacturing Outpost', 2705419591680, 112490618880, -982318571520, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014868, 500, 0, 50000000, 1000, 45, 12242, 150087495, 30001256, 20000183, 10000014, 'FAT-6P VIII - Moon 1 - Manufacturing Outpost', 1245813596160, -3994705920, 683998126080, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014870, 500, 0, 50000000, 1000, 45, 12242, 144684705, 30002617, 20000383, 10000031, 'Z-N9IP VII - Moon 2 - Manufacturing Outpost', 1744243384320, 58374266880, 444875120640, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014872, 500, 0, 50000000, 1000, 45, 12242, 150087495, 30003121, 20000456, 10000039, 'BZ-0GW V - Moon 1 - Manufacturing Outpost', -523215790080, 41777356800, 74517504000, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014873, 500, 0, 50000000, 1000, 45, 12242, 1000135, 30003666, 20000534, 10000045, 'V7-MID IX - Moon 1 - Manufacturing Outpost', -1771751546880, -224628203520, -2169463480320, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014883, 500, 0, 50000000, 1000, 45, 12242, 150094093, 30004962, 20000726, 10000063, 'G-Q5JU VII - Moon 1 - Manufacturing Outpost', -2462505246720, -125320519680, -207525027840, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014890, 500, 0, 50000000, 1000, 45, 12294, 1000135, 30000867, 20000127, 10000010, 'D7-ZAC VIII - Moon 1 - Refining Outpost', -36212121600, 1780162560, -483143884800, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014906, 500, 0, 50000000, 1000, 45, 12294, 150134971, 30004935, 20000722, 10000063, 'TPAR-G IX - Moon 3 - Refining Outpost', 546419466240, -91034296320, -769929584640, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014920, 500, 0, 50000000, 1000, 45, 12295, 150001295, 30003689, 20000537, 10000046, 'Allah Ackhbar', -2135999815680, 222355415040, -231166156800, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014921, 500, 0, 50000000, 1000, 45, 12295, 144693213, 30003979, 20000581, 10000050, '9CG6-H VIII - Moon 4 - Cloning Outpost', -482415452160, -32584704000, 289285939200, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014922, 500, 0, 50000000, 1000, 45, 12295, 1000135, 30004059, 20000593, 10000051, '9-4RP2 IV - Moon 2 - Cloning Outpost', 878666588160, 175879741440, -1675383644160, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014924, 500, 0, 50000000, 1000, 45, 12295, 1000135, 30004453, 20000650, 10000056, 'VNGJ-U V - Moon 1 - Cloning Outpost', -90715545600, 8120279040, 236737781760, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(60014926, 500, 0, 50000000, 1000, 45, 12295, 150134971, 30004712, 20000689, 10000060, 'NOL-M9 VI - Moon 2 - Cloning Outpost', 356915896320, 67440599040, -143501844480, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(61000001, 500, 0, 50000000, 1000, 45, 21646, 150020944, 30004470, 20000652, 10000056, 'DB1R-4 II - duperTum Corp Minmatar Service Outpost', 41858520043.9836, -3560717577.50918, -74433249981.7999, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(61000002, 500, 0, 50000000, 1000, 45, 21646, 150020944, 30004469, 20000652, 10000056, 'ZS-2LT XI - duperTum Corp Minmatar Service Outpost', 3551829704651.49, 48132245340.0623, 1676316680033.34, 0.5, 0.025, 4);
INSERT INTO `staStations` (`stationID`, `security`, `dockingCostPerVolume`, `maxShipVolumeDockable`, `officeRentalCost`, `operationID`, `stationTypeID`, `corporationID`, `solarSystemID`, `constellationID`, `regionID`, `stationName`, `x`, `y`, `z`, `reprocessingEfficiency`, `reprocessingStationsTake`, `reprocessingHangarFlag`) VALUES(61000003, 500, 0, 50000000, 1000, 45, 21646, 150020944, 30004952, 20000724, 10000063, 'JZL-VB VIII - duperTum Corp Minmatar Service Outpost', -369609154560, -38893363200, 2953009766400, 0.5, 0.025, 4);
/*
 * This SQL batch drops some columns which are no longer needed.
 * 
 * characterName, typeID -> LEFT JOIN entity
 * bloodlineID           -> LEFT JOIN entity, bloodlineTypes
 * raceID                -> LEFT JOIN entity, bloodlineTypes, chrBloodlines
 *
 * I know it may look difficult but we don't want to store (and manage)
 * duplicates, do we?
 */

ALTER TABLE character_
 DROP COLUMN characterName,
 DROP COLUMN typeID,
 DROP COLUMN bloodlineID,
 DROP COLUMN raceID;

/*
 * We don't drop characterStatic's columns characterName
 * and typeID because prime_db.sql uses these columns to
 * properly populate entity and eveStaticOwners tables.
 */  
ALTER TABLE characterStatic
 DROP COLUMN bloodlineID,
 DROP COLUMN raceID;

/*
 * Moving corporation roles from chrCorporationRoles to character_.
 */

ALTER TABLE character_
 ADD COLUMN corpRole BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER corporationID,
 ADD COLUMN rolesAtAll BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER corpRole,
 ADD COLUMN rolesAtBase BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER rolesAtAll,
 ADD COLUMN rolesAtHQ BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER rolesAtBase,
 ADD COLUMN rolesAtOther BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' AFTER rolesAtHQ
;

DROP TABLE IF EXISTS chrCorporationRoles;
DROP TABLE IF EXISTS `chrSkillQueue`;

CREATE TABLE `chrSkillQueue` (
	`id` int(10) unsigned NOT NULL auto_increment,
	`itemID` int(10) unsigned NOT NULL,
	`typeID` int(10) unsigned NOT NULL,
	`level` int(10) unsigned NOT NULL,
	UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED;
ALTER TABLE `chrSkillQueue`
 CHANGE `id` `orderIndex` INT(10) UNSIGNED NOT NULL AFTER `characterID`,
 CHANGE `itemID` `characterID` INT(10) UNSIGNED NOT NULL,
 DROP KEY `id`;
