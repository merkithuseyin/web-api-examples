package businesslogic

import (
	"example/gin/core/models"
	dataaccess "example/gin/data-access"
)

type PairBLL struct {
	PairRepo dataaccess.PairDAL
}

func (p *PairBLL) Get(id int) (models.Pair, error) {
	return p.PairRepo.Get(id)
}

func (p *PairBLL) GetAll() []models.Pair {
	return p.PairRepo.GetAll()
}

func (p *PairBLL) Save(pair models.Pair) {
	p.PairRepo.Save(pair)
}

func (p *PairBLL) Update(id int, pair models.Pair) error {
	return p.PairRepo.Update(id, pair)
}

func (p *PairBLL) Delete(id int) error {
	return p.PairRepo.Delete(id)
}
