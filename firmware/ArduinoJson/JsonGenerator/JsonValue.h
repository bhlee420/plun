/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "EscapedString.h"
#include "Printable.h"
#include "StringBuilder.h"

namespace ArduinoJson
{
    namespace Internals
    {
        class JsonValue
        {
        public:

            void set(bool value)
            {
                printToImpl = &printBoolTo;
                content.asBool = value;
            }

            void set(long value)
            {
                printToImpl = &printLongTo;
                content.asLong = value;
            }

            void set(int value)
            {
                printToImpl = &printLongTo;
                content.asLong = value;
            }

            void set(Printable& value)
            {
                printToImpl = &printPrintableTo;
                content.asPrintable = &value;
            }

            void set(const char* value)
            {
                printToImpl = &printStringTo;
                content.asString.set(value);
            }

            void set(double value)
            {
                set<2>(value);
            }

            template <int DIGITS>
            void set(double value)
            {
                printToImpl = &printDoubleTo<DIGITS>;
                content.asDouble = value;
            }

            size_t printTo(Print& p) const
            {
                // handmade polymorphism
                return printToImpl(content, p);
            }

        private:
            union Content
            {
                bool asBool;
                long asLong;
                Printable* asPrintable;
                EscapedString asString;
                double asDouble;
            };

            Content content;

            size_t(* printToImpl)(const Content&, Print&);

            static size_t printBoolTo(const Content&, Print&);
            static size_t printLongTo(const Content&, Print&);
            static size_t printPrintableTo(const Content&, Print&);
            static size_t printStringTo(const Content&, Print&);

            template <int DIGITS>
            static size_t printDoubleTo(const Content& c, Print& p)
            {
                return p.print(c.asDouble, DIGITS);
            }
        };
    }
}