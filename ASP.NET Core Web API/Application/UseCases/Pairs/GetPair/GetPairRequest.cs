using Application.Interfaces;
using Domain;
using MediatR;

namespace Application.UseCases.Pairs.GetPair;

public record GetPairRequest(int Id): IRequest<GetPairResponse>, IRequestValidator
{
    public void Validate()
    {
        if (Id <= 0)
        {
            throw new System.ArgumentException(StringResources.Id_Can_Not_Be_Non_Positive_Number);
        }
    }
}