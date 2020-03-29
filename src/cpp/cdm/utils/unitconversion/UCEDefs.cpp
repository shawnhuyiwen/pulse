#include "stdafx.h"
#include "utils/unitconversion/UnitConversionEngine.h"

// This originally started out as a file read from disk
// But this file rarely changed, and when it did, 
// we needed to recompile anyway (adding to an SEScalar class)
// It was a pain having to have a file in a particular place at
// startup time due to the need for UCE to be a singleton.
// So its better to get this file off disk and into code
// Yes, I am just stringifying the file.
// But, there is something to be said to preserving
// the configuration format as it was for editing units

#define UCE(entry) m_UCEdefs << #entry << std::endl;

void CUnitConversionEngine::GetUCEdefs()
{
  if (!m_UCEdefs.str().empty())
    return;

  // Prefix format :
  // PREFIX <name> <symbol> <conversion - factor>
  // [note that symbol is single char only]
  //
  // Quantity formats :
  // 1) QUANTITY FUNDAMENTAL <name>[20LOG]
  // 2) QUANTITY DERIVED <name>[20LOG] <expansion>
  //  <expansion> is a string representation of a complex unit. Juxtaposed symbols must be whitespace-separated.
  //    Symbols must already be defined
  //      Note that each quantity definition implicitly defines an anonymous, "base unit" for that type.
  //    The base unit refers to any subsequently - defined unit that has a conversion - factor of 1.0.For fundamental
  //       types, this is also the unit's bigness. For derived types, the bigness of any unit is its conversion
  //    factor times the bigness of the derived type's expansion. The unit conversion engine does not require
  //    the existence of a named unit whose conversion factor is 1.0, though for fundamental types, there is
  //    really no reason for there not to be one.
  //  [20LOG] is an optional flag that indicates that this quantity should obey the "20 Log Rule",
  //    for decibels, as explained in http ://en.wikipedia.org/wiki/Decibel
  //
  // Unit formats
  // 1) UNIT <quantity - name> <name> <symbol> <conversion - factor> <expansion>    {ALLPREFIXES / NOPREFIXES}[<bias>]
  // 2) UNIT <quantity - name> <name> <symbol> <conversion - factor> <expansion>  {GOODPREFIXES / BADPREFIXES} <prefix - char - seq>[<bias>]
  //   [bias] is at the end because it is optional and seldom used (e.g. temperature)
  //   <prefix-char-seq> is a sequence of prefix characters that are either allowed or forbidden
  //  <quantity-name> is the name of the previously-defined quantity type to which this unit applies.
  //   <expansion> is a complex unit with respect to which the current unit's conversion factor and bias are defined, or "*"
  //     if wrt the base unit. A note on biases: Whereas conversion factors are
  //    relative, and the choice of base unit is largely arbitrary as far as conversion factors are concerned,
  //    the same is not true with biases.In order for quantity type conversions to be done properly
  //    (e.g. Temperature to Energy and vice versa), the base unit must be an absolute scale (e.g.
  //    (Kelvin or Rankine). Selecting Celsius as the base unit, and specifying biases relative to
  //    it, may work for simple conversions from one temperature to another, but will not yield
  //    correct results when converting between quantity types.This restriction does not
  //    preclude defining such a unit's bias and conversion factor in terms of some other
  //    non - base unit, so long as the definition chain ends with a unit of absolute scale.Note that
  //    the specified bias value is the value of the current unit when the target unit has the
  //    value zero.
  //
  // Quantity type mapping format :
  // TYPEMAP <to - quantity> <from - quantity> <from - exponent> <mapping - unit>
  //  TYPEMAP allows the conversion between disparate quantity types.Such conversions are almost
  //  always context dependent.For example, one can convert frequency to energy in the context of
  //  electromagnetic radiation by multiplying the frequency(raised to the first power) by Planck's
  //  constant(raised to the first power).
  //  <to-quantity> is the name of the quantity being converted to (e.g. Energy)
  //  <from-quantity> is the name of the quantity type being converted from (e.g. Frequency).
  //  <from-exponent> is the power that the original value needs to be raised to. (In the case of E = h nu,
  //    this power is 1.0, but some conversions might involve an inverse relationship, in which
  //    case the power might be - 1.0.)
  //  <mapping-unit> is an arbitrary compound unit string representing
  //    the physical constant or constants, such as "c", or "h", or "c^2", or "c h".
  //  Note that there is no need to define an inverse typemap in the other direction.This is computed automatically.

  // A note on decibels
  // Compled units may be preceded with "dB " to signify values in decibels relative to the unit which follows.
  // E.g., you can use a unit like "dB mW", or "dB V/m".

  UCE(PREFIX  yotta  Y  1.0e24)
  UCE(PREFIX  zetta  Z  1.0e21)
  UCE(PREFIX  exa    E  1.0e18)
  UCE(PREFIX  peta   P  1.0e15)
  UCE(PREFIX  tera   T  1.0e12)
  UCE(PREFIX  giga   G  1.0e9)
  UCE(PREFIX  mega   M  1.0e6)
  UCE(PREFIX  kilo   k  1.0e3)
  UCE(PREFIX  hecto  h  1.0e2)
  UCE(PREFIX  deci   d  1.0e-1)
  UCE(PREFIX  centi  c  1.0e-2)
  UCE(PREFIX  milli  m  1.0e-3)
  UCE(PREFIX  micro  u  1.0e-6)
  UCE(PREFIX  nano   n  1.0e-9)
  UCE(PREFIX  pico   p  1.0e-12)
  UCE(PREFIX  femto  f  1.0e-15)
  UCE(PREFIX  atto   a  1.0e-18)
  UCE(PREFIX  zepto  z  1.0e-21)
  UCE(PREFIX  yocto  y  1.0e-24)

  UCE(QUANTITY  FUNDAMENTAL  Time)
  UCE(UNIT  Time    second          s          1.0         *    GoodPrefixes munpfazy)
  UCE(UNIT  Time    minute          min        60.0        s    NoPrefixes)
  UCE(UNIT  Time    hour            hr         60.0        min  NoPrefixes)
  UCE(UNIT  Time    day             day        24.0        hr   NoPrefixes)
  UCE(UNIT  Time    year            yr         365.242199  day  NoPrefixes)
  UCE(UNIT  Time    fortnight       fortnight  14          day  NoPrefixes)

  UCE(QUANTITY  FUNDAMENTAL  Distance)
  UCE(UNIT  Distance  meter         m          1.0         *    BadPrefixes MGTPEZY)
  UCE(UNIT  Distance  inch          in         2.54        cm   NoPrefixes)
  UCE(UNIT  Distance  mil           mil        0.001       in   NoPrefixes)
  UCE(UNIT  Distance  foot          ft         12          in   GoodPrefixes k)
  UCE(UNIT  Distance  yard          yd         3           ft   NoPrefixes)
  UCE(UNIT  Distance  mile          mi         5280        ft   NoPrefixes)
  UCE(UNIT  Distance  nautical-mile nmi        1852        m    NoPrefixes)
  UCE(UNIT  Distance  furlong       furlong    0.125       mi   NoPrefixes)
  UCE(UNIT  Distance  chain         chain      22          yd   NoPrefixes)
  UCE(UNIT  Distance  rod           rod        16.5        ft   NoPrefixes)
  UCE(UNIT  Distance  fathom        fathom     6           ft   NoPrefixes)
  UCE(UNIT  Distance  cubit         cubit      18          in   NoPrefixes)

  UCE(QUANTITY  FUNDAMENTAL  Osmole)
  UCE(UNIT  Osmole    osmole        Osm        1.0         *    GoodPrefixes m)

  // A radian is 1 / 2pi, or .5 / pi, of a cycle.The derived type is m / m because it's a dimensionless
  // ratio of the length of an arc to the length of an entire circumference.
  // We define this way, instead of the standard definition of a radian as 1.0 m of
  // arc per 1.0 m radius, so that we can interconvert "rad/s" and "Hz" (= 1 / s).
  // With this definition "1 Hz" converts to "2PI rad/s", which is what we want.
  // the rad conversion factor is 1 / 2Pi, and the deg conversion factor is 1 / 360.
  // Some of the other units, like pi, degpercyc, and degfac, are merely hacks to simplify the
  // arithmetic expressions so that I don't need to put decimal expansions for 1/2pi,
  // 1 / 60, or 1 / 60.
  UCE(QUANTITY  DERIVED  PlaneAngle  m/m)
  UCE(UNIT  PlaneAngle  pi          pi         3.1415926535897932384626433832795  *             NoPrefixes)
  UCE(UNIT  PlaneAngle  degpercyc   degpercyc  360.0                              *             NoPrefixes)
  // Degfac is used for computing and defining minutes and seconds of arc
  UCE(UNIT  PlaneAngle  degfac      degfac    60.0                                *             NoPrefixes)
  // Note how we define a radian as being 1 / 2pi through a combination of the factor 0.5 and the unit "pi"
  UCE(UNIT  PlaneAngle  radian      rad       0.5                                 1/pi          GoodPrefixes cmu)
  UCE(UNIT  PlaneAngle  degree      deg       1.0                                 1/degpercyc   NoPrefixes)
  UCE(UNIT  PlaneAngle  minute-arc  minarc    1.0                                 deg/degfac    NoPrefixes)
  UCE(UNIT  PlaneAngle  second-arc  secarc    1.0                                 minarc/degfac NoPrefixes)

  UCE(QUANTITY  DERIVED    Frequency  1/s)
  UCE(UNIT  Frequency  hertz    Hz  1.0    *  GoodPrefixes kMGT)

  UCE(QUANTITY  FUNDAMENTAL  AmountOfSubstance)
  UCE(UNIT  AmountOfSubstance  mole    mol  1.0    *  AllPrefixes)

  // Avogadro's constant is the number of things in a mole of things, or simply "inverse moles". 
  UCE(QUANTITY  DERIVED    InverseAmount  1/mol)
  UCE(UNIT  InverseAmount  AvogadrosConstant  Na  6.02214179E23  *  NoPrefixes)

  // Now that we have Avogadro's constant, define other amounts
  UCE(UNIT  AmountOfSubstance  count    ct    1.0      Na^-1  NoPrefixes)
  UCE(UNIT  AmountOfSubstance  dozen    doz   12       ct     NoPrefixes)

  UCE(QUANTITY  FUNDAMENTAL  Mass)
  UCE(UNIT  Mass  gram        g      1.0           *         AllPrefixes)
  // The definition of an AMU looks weird, but "mol Na" gives Avogadro's constant as a 
  // unitless number, which is the correct number of amu's in a gram.
  UCE(UNIT  Mass  UnifiedAMU  u      1.0           g/mol Na  NoPrefixes)
  UCE(UNIT  Mass  dalton      Da     1.0           u         GoodPrefixes k)
  UCE(UNIT  Mass  AMU         amu    1.0           u         NoPrefixes)
  UCE(UNIT  Mass  ounce       oz     28.349523125  g         NoPrefixes)
  UCE(UNIT  Mass  pound       lb     16            oz        NoPrefixes)
  UCE(UNIT  Mass  stone       stone  14            lb        NoPrefixes)
  UCE(UNIT  Mass  ton         ton    2000.0        lb        NoPrefixes)

  UCE(QUANTITY  FUNDAMENTAL  20LOG Current)
  UCE(UNIT  Current  ampere    A  1.0    *  AllPrefixes)

  UCE(QUANTITY  FUNDAMENTAL  LuminousIntensity)
  UCE(UNIT  LuminousIntensity  candela    cd  1.0    *  AllPrefixes)

  // Defining Rankine as base unit because it's easier to specify a conversion
  // factor of 1.8 than 0.555555555555555555.When we do this in XML, we'll allow
  // full expressions for conversion factors, so we can just say(5.0 / 9.0).
  UCE(QUANTITY   FUNDAMENTAL  Temperature)
  UCE(UNIT  Temperature  DegreesRankine    degR  1.0              *     AllPrefixes  0.0)
  UCE(UNIT  Temperature  Kelvin            K     1.8              degR  AllPrefixes  0.0)
  UCE(UNIT  Temperature  DegreesCelsius    degC  1.0              K     AllPrefixes -273.15)
  UCE(UNIT  Temperature  DegreesFahrenheit degF  0.5555555555555  degC  NoPrefixes   32.0)

  UCE(QUANTITY  DERIVED    Area    m^2)
  UCE(UNIT  Area    acre      acre    4046.85642  *    NoPrefixes)
  UCE(UNIT  Area    are       are     100         *    NoPrefixes)
  UCE(UNIT  Area    decare    decare  10          are  NoPrefixes)
  UCE(UNIT  Area    barn      b       100         fm^2 NoPrefixes)

  UCE(QUANTITY  DERIVED  Volume  m^3)
  UCE(UNIT  Volume    liter             l       0.001      *     GoodPrefixes mu)
  UCE(UNIT  Volume    Liter             L       1.0        l     GoodPrefixes mud)
  UCE(UNIT  Volume    gallon            gal     3.7854118  l     NoPrefixes)
  UCE(UNIT  Volume    quart             qt      0.25       gal   NoPrefixes)
  UCE(UNIT  Volume    pint              pt      0.5        qt    NoPrefixes)
  UCE(UNIT  Volume    cubic-centimeter  cc      1.0        cm^3  NoPrefixes)
  UCE(UNIT  Volume    cubic-foot        cf      1.0        ft^3  NoPrefixes)
  UCE(UNIT  Volume    peck              peck    2          gal   NoPrefixes)
  UCE(UNIT  Volume    bushel            bushel  4          peck  NoPrefixes)
  UCE(UNIT  Volume    hogshead          hhd     63         gal   NoPrefixes)
  UCE(UNIT  Volume    barrel            brl     31.5       gal   NoPrefixes)

  UCE(QUANTITY  DERIVED    Force    kg m / s ^ 2)
  UCE(UNIT  Force    newton       N    1.0         *  AllPRefixes)
  UCE(UNIT  Force    dyne         dyn  1.0e-5      N  AllPrefixes)
  UCE(UNIT  Force    pound-force  lbf  4.44822162  N  NoPrefixes)

  UCE(QUANTITY  DERIVED    Energy    N m)
  UCE(UNIT  Energy    joule               J    1.0         *     BadPRefixes c)
  UCE(UNIT  Energy    erg                 erg  1.0e-7      J     AllPrefixes)
  UCE(UNIT  Energy    calorie             cal  4.184       J     AllPrefixes)
  UCE(UNIT  Energy    Calorie             Cal  1.0         kcal  AllPrefixes)
  // "t" is a tonne (metric) of TNT. By convention, a gram of TNT is 1000 cal. Thus a tonne of TNT is 1 Gcal
  UCE(UNIT   Energy   tonne-of-TNT        t    1.0         Gcal  GoodPrefixes umkMG)
  UCE(UNIT   Energy   BritishThermalUnit  BTU  1055.05585  J     NoPrefixes)

  UCE(QUANTITY  DERIVED    Power    J/s)
  UCE(Unit  Power    watt        W   1.0         *  AllPrefixes)
  UCE(Unit  Power    horsepower  hp  745.699872  W  GoodPRefixes k)

  UCE(QUANTITY  DERIVED  20LOG  Speed    m/s)
  UCE(UNIT  Speed    lightspeed    c    2.99792458e+8  *       NoPrefixes)
  UCE(UNIT  Speed    speedoflight  lt   1.0            c       NoPrefixes)
  UCE(UNIT  Speed    milesperhour  mph  1.0            mi/hr   NoPrefixes)
  UCE(UNIT  Speed    knots         kn   1.0            nmi/hr  NoPrefixes)

  UCE(QUANTITY  DERIVED    Acceleration  m/s^2)
  UCE(UNIT  Acceleration  gravity    grav  9.8    *  NoPrefixes)

  UCE(QUANTITY  DERIVED    Pressure  N/m^2)
  UCE(UNIT  Pressure  pascal      Pa     1.0         *         AllPrefixes)
  UCE(UNIT  Pressure  bar         bar    1.0e+5      Pa        AllPrefixes)
  UCE(UNIT  Pressure  torr        Torr   133.322368  Pa        AllPrefixes)
  UCE(UNIT  Pressure  atmosphere  atm    760         Torr      AllPrefixes)
  UCE(UNIT  Pressure  psi         psi    1.0         lbf/in^2  NoPrefixes)
  UCE(UNIT  Pressure  mmHg        mmHg   1.0         Torr      NoPrefixes)
  UCE(UNIT  Pressure  cmH2O       cmH2O  98.0665     Pa        NoPrefixes)

  UCE(QUANTITY  DERIVED  20LOG  ElectricCharge  A s)
  UCE(UNIT  ElectricCharge  coulomb            C  1.0             *  AllPrefixes)
  UCE(UNIT  ElectricCharge  elementary-charge  e  1.60217646E-19  C  NoPrefixes)

  UCE(QUANTITY  DERIVED  20LOG  ElectricPotential  W/A)
  UCE(UNIT  ElectricPotential  volt    V  1.0    *  AllPrefixes)

  // Now I can define an electron-volt in terms of existing units!
  UCE(UNIT  Energy    electron-volt  eV  1.0   e V  AllPrefixes)

  UCE(QUANTITY  DERIVED    Capacitance  C/V)
  UCE(UNIT  Capacitance  farad    F  1.0    *  AllPrefixes)

  UCE(QUANTITY  DERIVED    Resistance  V/A)
  UCE(UNIT  Resistance  ohm    ohm  1.0    *       AllPrefixes)
  // Free space impedance, used by conversion of electric field strength to power density.Exact value is 120 pi ohms.
  UCE(UNIT  Resistance  Z0     Z0   120    pi ohm  NoPrefixes)

  // Insulation coefficient(http://orefa.net/insulation_coefficient.html)
  // Thermal Resistance with Area
  UCE(QUANTITY  DERIVED  ThermalResistanceArea  m^2 L/W)
  UCE(UNIT  ThermalResistanceArea rsi     rsi     1.0           *    NoPrefixes)
  UCE(UNIT  ThermalResistanceArea clo     clo     0.155         rsi  NoPrefixes)
  UCE(UNIT  ThermalResistanceArea rValue  rValue  0.1761101838  rsi  NoPrefixes)
  UCE(UNIT  ThermalResistanceArea tog     tog     0.1           rsi  NoPrefixes)

  UCE(QUANTITY  DERIVED    Conductance  A / V)
  UCE(UNIT  Conductance  siemens    S  1.0    *  AllPrefixes)

  UCE(QUANTITY  DERIVED  20LOG  MagneticFlux  V s)
  UCE(UNIT  MagneticFlux  weber    Wb  1.0    *  AllPrefixes)

  UCE(QUANTITY  DERIVED  20LOG  MagneticFluxDensity  Wb/m^2)
  UCE(UNIT  MagneticFluxDensity  tesla    T  1.0       *  AllPrefixes)
  UCE(UNIT  MagneticFluxDensity  gauss    G  0.0001    T  AllPrefixes)

  UCE(QUANTITY  DERIVED    Inductance  Wb/A)
  UCE(UNIT  Inductance  henry    H  1.0    *  AllPrefixes)

  // Conversion factor is 1 / 4Pi, fraction of sphere subtended by steradian
  UCE(QUANTITY  DERIVED    SolidAngle  m^2/m^2)
  UCE(UNIT  SolidAngle  steradian  sr  .25    1/pi   GoodPrefixes cmun)

  UCE(QUANTITY  DERIVED    LuminousFlux  cd sr)
  UCE(UNIT  LuminousFlux  lumen    lm  1.0    *  AllPrefixes)

  UCE(QUANTITY  DERIVED    Illuminance  lm / m ^ 2)
  UCE(UNIT  Illuminance  lux         lx  1.0    *        AllPrefixes)
  UCE(UNIT  Illuminance  footcandle  fc  1.0    lm/ft^2  AllPrefixes)

  UCE(QUANTITY  DERIVED    Luminance cd/m^2)
  UCE(UNIT  Luminance  nit          nit  1.0    *           AllPrefixes)
  UCE(UNIT  Luminance  stilb        sb   1.0    cd/cm^2     AllPrefixes)
  UCE(UNIT  Luminance  lambert      Lb   1.0    sb/pi       AllPrefixes)
  UCE(UNIT  Luminance  footlambert  fL   1.0    cd/pi ft^2  AllPrefixes)

  UCE(QUANTITY  DERIVED    ActivityRadionuclide  1/s)
  UCE(UNIT  ActivityRadionuclide  becquerel  Bq  1.0    *  AllPrefixes)


  UCE(QUANTITY  DERIVED    Radiation-Dose  J/kg)
  UCE(UNIT  Radiation-Dose  gray    Gy  1.0   *   AllPrefixes)
  UCE(UNIT  Radiation-Dose  rad     rd  0.01  Gy  NoPrefixes)

  // Define quantity types whose sole purpose is to identify the type of a unit
  // corresponding to physical constants
  // Defining physical constants as units in their respective quantity types
  // allows quantity type conversion via unit composition

  UCE(QUANTITY  DERIVED    Action    J s)
  UCE(UNIT  Action    plancks-constant h  6.6260689633E-34 *  NoPrefixes)

  UCE(QUANTITY  DERIVED    AngularAction  h / pi)
  UCE(UNIT  AngularAction  diracs-constant  hbar  1.0    *  NoPrefixes)

  // It might be neat to be able to convert "moles of mass" to "mass".For example,
  // I could take the mass of an oxygon atom as "16 AMU" and multiply that by the
  // amount of oxygen I have, say "3 moles", and convert the result to a
  // mass - only.Such a conversion would be from a quantity that has dimensions
  // of mass times moles, so we need to define such a quantity.The precise
  // units we use to define it(gram moles, pound - moles, etc) is immaterial.
  // We don't need any special units of this quantity defined, it just needs
  // a name(and correctly - dimensioned expansion, of course) for the TYPEMAP.
  UCE(QUANTITY  DERIVED    MolesOfMass  mol g)

  // IWMDT allows conversion of electric field strength at an antenna to power density
  // so define these quantities so that we can define the conversion TYPEMAP later
  UCE(QUANTITY   DERIVED  20LOG  ElectricFieldStrength  V/m)
  UCE(QUANTITY   DERIVED         PowerDensity           W/cm^2)

  //Sometimes it's convenient to express temperature as energy using Boltzmann's constant
  UCE(QUANTITY  DERIVED    BoltzmannQuantity  J/K)
  UCE(UNIT  BoltzmannQuantity  BoltzmannConstant  kB  1.3806503E-23 *  NoPrefixes)

  //
  // Define the mappings from one quantity type to another
  //

  // Mass to Energy : E = mc ^ 2
  UCE(TYPEMAP   Energy         Mass                   1  c^2)
  // Frequency to Energy : E = h nu(for photons)
  UCE(TYPEMAP    Energy        Frequency              1  h)
  // Frequency to Wavelength : lambda = c / nu(for photons)
  UCE(TYPEMAP    Distance      Frequency             -1  c)
  // Wavelength to Energy : E = hc / lambda
  UCE(TYPEMAP    Energy        Distance              -1  h c)
  // Moles of mass to mass : Multiply by Avogadro's constant
  UCE(TYPEMAP    Mass          MolesOfMass            1  Na)
  // Electric field strength to power density : PD = E^2/Z0
  UCE(TYPEMAP    PowerDensity  ElectricFieldStrength  2  1/Z0)
  // Temperature to average kinetic energy of particle
  UCE(TYPEMAP    Energy        Temperature            1  kB)

  m_UCEdefs.seekg(0, std::ios::beg);
}