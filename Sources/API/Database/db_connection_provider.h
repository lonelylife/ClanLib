/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/


#pragma once

#include "api_database.h"
#include "db_command.h"
#include "db_transaction.h"

namespace clan
{
/// \addtogroup clanDatabase_System clanDatabase System
/// \{

class DBCommandProvider;
class DBTransactionProvider;
class DBReaderProvider;

/// \brief Database connection provider.
class CL_API_DATABASE DBConnectionProvider
{
/// \name Construction
/// \{
public:
	virtual ~DBConnectionProvider() { return; }
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	/// \brief Create database command.
	virtual DBCommandProvider *create_command(const std::string &text, DBCommand::Type type) = 0;

	/// \brief Begin a transaction.
	virtual DBTransactionProvider *begin_transaction(DBTransaction::Type type) = 0;

	/// \brief Begin execution of database command.
	virtual DBReaderProvider *execute_reader(DBCommandProvider *command) = 0;

	/// \brief Execute database command and returns the first column of the first row.
	virtual std::string execute_scalar_string(DBCommandProvider *command) = 0;

	/// \brief Execute database command and returns the first column of the first row.
	virtual int execute_scalar_int(DBCommandProvider *command) = 0;

	/// \brief Execute database command.
	virtual void execute_non_query(DBCommandProvider *command) = 0;
/// \}

/// \name Implementation
/// \{
private:
/// \}
};

}

/// \}
