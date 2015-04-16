/*
 * Copyright 2013, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef PTC_ABC_PARTICLES_H
#define PTC_ABC_PARTICLES_H

#include <Alembic/AbcGeom/IPoints.h>
#include <Alembic/AbcGeom/OPoints.h>
#include <Alembic/AbcGeom/ICurves.h>
#include <Alembic/AbcGeom/OCurves.h>

#include "ptc_types.h"

#include "PTC_api.h"

#include "abc_reader.h"
#include "abc_schema.h"
#include "abc_writer.h"
#include "abc_cloth.h"

struct ListBase;
struct Object;
struct ParticleSystem;
struct ParticleCacheKey;
struct Strands;
struct StrandsChildren;

namespace PTC {

class AbcDerivedMeshWriter;
class AbcDerivedMeshReader;


class AbcStrandsChildrenWriter : public AbcWriter {
public:
	AbcStrandsChildrenWriter(const std::string &name, const std::string &abc_name, DupliObjectData *dobdata);
	
	StrandsChildren *get_strands() const;
	
	void init_abc(Abc::OObject parent);
	
	void write_sample();
	
private:
	std::string m_name;
	std::string m_abc_name;
	DupliObjectData *m_dobdata;
	
	AbcGeom::OCurves m_curves;
	AbcGeom::OM33fArrayProperty m_prop_root_matrix;
	AbcGeom::OV3fArrayProperty m_prop_root_positions;
	AbcGeom::OFloatGeomParam m_param_times;
	AbcGeom::OInt32ArrayProperty m_prop_parents;
	AbcGeom::OFloatArrayProperty m_prop_parent_weights;
};


class AbcStrandsWriter : public AbcWriter {
public:
	AbcStrandsWriter(const std::string &name, DupliObjectData *dobdata);
	
	Strands *get_strands() const;
	
	void init(WriterArchive *archive);
	void init_abc(Abc::OObject parent);
	
	void write_sample();
	
private:
	std::string m_name;
	DupliObjectData *m_dobdata;
	
	AbcGeom::OCurves m_curves;
	AbcGeom::OM33fGeomParam m_param_root_matrix;
	AbcGeom::OFloatGeomParam m_param_times;
	AbcGeom::OFloatGeomParam m_param_weights;
	AbcGeom::OCompoundProperty m_param_motion_state;
	AbcGeom::OP3fGeomParam m_param_motion_co;
	AbcGeom::OV3fGeomParam m_param_motion_vel;
	
	AbcStrandsChildrenWriter m_child_writer;
};


class AbcStrandsChildrenReader : public AbcReader {
public:
	AbcStrandsChildrenReader(StrandsChildren *strands);
	~AbcStrandsChildrenReader();
	
	void init_abc(Abc::IObject object);
	
	PTCReadSampleResult read_sample(float frame);
	
	StrandsChildren *get_result() { return m_strands; }
	StrandsChildren *acquire_result();
	void discard_result();
	
private:
	StrandsChildren *m_strands;
	
	AbcGeom::ICurves m_curves;
	AbcGeom::IM33fArrayProperty m_prop_root_matrix;
	AbcGeom::IV3fArrayProperty m_prop_root_positions;
	AbcGeom::IFloatGeomParam m_param_times;
	AbcGeom::IInt32ArrayProperty m_prop_parents;
	AbcGeom::IFloatArrayProperty m_prop_parent_weights;
};


class AbcStrandsReader : public AbcReader {
public:
	AbcStrandsReader(Strands *strands, StrandsChildren *children, bool read_motion, bool read_children);
	~AbcStrandsReader();
	
	void init(ReaderArchive *archive);
	void init_abc(Abc::IObject object);
	
	PTCReadSampleResult read_sample(float frame);
	
	Strands *acquire_result();
	void discard_result();
	
	AbcStrandsChildrenReader &child_reader() { return m_child_reader; }
	
private:
	bool m_read_motion, m_read_children;
	Strands *m_strands;
	
	AbcGeom::ICurves m_curves;
	AbcGeom::IM33fGeomParam m_param_root_matrix;
	AbcGeom::IFloatGeomParam m_param_times;
	AbcGeom::IFloatGeomParam m_param_weights;
	AbcGeom::ICompoundProperty m_param_motion_state;
	AbcGeom::IP3fGeomParam m_param_motion_co;
	AbcGeom::IV3fGeomParam m_param_motion_vel;
	
	AbcStrandsChildrenReader m_child_reader;
};


} /* namespace PTC */

#endif  /* PTC_PARTICLES_H */
