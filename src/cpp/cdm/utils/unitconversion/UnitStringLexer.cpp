/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

//----------------------------------------------------------------------------
/// @file UnitStringLexer.cpp
/// @author Chris Volpe
//----------------------------------------------------------------------------

#include "cdm/CommonDefs.h"
#include "cdm/utils/unitconversion/UCCommon.h"

void CUnitStringLexer::ScanNextToken()
{
  bool done = false;
  curToken.tokenID = SUnitStringToken::TT_EOF;

  // Bail out with an EOF if there's nothing left.
  if (cursor == end)
  {
    return;
  }

  do 
  {

    // A slash is used to indicate units in the denominator, but can also
    // be used to signify a fractional exponent
    if (*cursor == '/')
    {
      // always exit loop with cursor pointing to next character we want to read
      ++cursor;
      curToken.tokenID = SUnitStringToken::TT_SLASH;
      done = true;
    }

    else if (*cursor == '^')
    {
      ++cursor;
      curToken.tokenID = SUnitStringToken::TT_EXPONENT;
      done = true;
    }

    else if (*cursor == '(')
    {
      ++cursor;
      curToken.tokenID = SUnitStringToken::TT_PARENOPEN;
      done = true;
    }

    else if (*cursor == ')')
    {
      ++cursor;
      curToken.tokenID = SUnitStringToken::TT_PARENCLOSE;
      done = true;
    }

    // Check for the start of a number.
    else if (isdigit(*cursor) || (*cursor == '+') || (*cursor == '-') || 
          (*cursor == '.'))
    {
      // We'll use atof to convert the number, but then we'll advance the
      // cursor past all characters that could be used in a floating point number
      // even if the result would not be a floating point number (e.g. 3.1E10-+E9).
      // This discrepancy won't have any effect for well-formed input, but may result
      // in silent acceptance for some bad input. But it beats the two alternatives of
      // 1. converting the whole damn string to a stream and reading from the stream,
      // and 2. implementing the finite state machine for proper lexical analysis 
      // of floating point numbers. 
      curToken.tokenID = SUnitStringToken::TT_FLOAT;
      curToken.fltval = atof(&(*cursor));
      done = true;
      // I have no idea what the difference between "E" and "D" is in the
      // scientific notation exponent, but the documentation for atof() says
      // it's valid. 
      while ((cursor != end) && (isdigit(*cursor) || (*cursor=='+') || (*cursor == '-') ||
        (*cursor == '.') || (toupper(*cursor)=='E') || (toupper(*cursor)=='D')))
      {
        ++cursor;
      }
    }

    // Check for unit symbol
    else if (isalpha(*cursor))
    {
      std::string::iterator startSym = cursor;
      curToken.tokenID = SUnitStringToken::TT_IDENTIFIER;
      done = true;
      while ((cursor != end) && (isalpha(*cursor) || isdigit(*cursor)))
      {
        ++cursor;
      }
      // Note: second iterator in assign function points to one past last char
      // to copy, so cursor is properly positioned now as-is.
      curToken.symbol.assign(startSym,cursor);

      // Handle decibel (dB) token as a special case
      if (curToken.symbol == "dB")
      {
        curToken.tokenID = SUnitStringToken::TT_DB;
      }
    }

    else if (!isspace(*cursor))
    {
      // If it's not one of the above, and it's not a whitespace character,
      // then it's an error
      curToken.tokenID = SUnitStringToken::TT_ERR;
      done = true;
      ++cursor;
    }

    else
    {
      // continue looking
      ++cursor;
    }

  } while (!done && (cursor != end));
}
